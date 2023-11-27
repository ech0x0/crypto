#include "sha256.h"

#include <stdlib.h>

//swap from little to big endian
#define byteSwap64(x)                                                      \
	((((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | \
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) |  \
	 (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |  \
	 (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000))

static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define rotr(x, n) ((x >> n) | (x << (64 - n)))
#define shr(x, n) (x >> n)
#define ch(x, y, z) ((x & y) ^ ((~x) & z))
#define maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define Sig0(x) (rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22))
#define Sig1(x) (rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25))
#define sig0(x) (rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3))
#define sig1(x) (rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10))

static void compute_block(uint32_t* hash, const uint8_t* input) {
    uint32_t w[64];
    for (int i = 0; i < 16; ++i) {
        w[i] =
            ((uint32_t)input[0] << 24) |
            ((uint32_t)input[1] << 16) |
            ((uint32_t)input[2] << 8) |
            ((uint32_t)input[3]);
        input += 4;
    }
    for (int i = 16; i < 64; ++i) {
        w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];
    }

    uint32_t a = hash[0], b = hash[1], c = hash[2], d = hash[3], e = hash[4], f = hash[5], g = hash[6], h = hash[7];
    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + Sig1(e) + ch(e, f, g) + k[i] + w[i];
        uint32_t t2 = Sig0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
}

int sha256(uint32_t* hash, const uint8_t* message, const size_t message_len) {
    if (!hash) return 1;
    if (!message) return 2;
    if (message_len <= 0) return 3;

    hash[0] = 0x6a09e667;
    hash[1] = 0xbb67ae85;
    hash[2] = 0x3c6ef372;
    hash[3] = 0xa54ff53a;
    hash[4] = 0x510e527f;
    hash[5] = 0x9b05688c;
    hash[6] = 0x1f83d9ab;
    hash[7] = 0x5be0cd19;

    size_t index = 0;
    while ((index + 1) * 512 <= message_len) {
        compute_block(hash, message + index * 64);
        ++index;
    }

    //pad and compute final blocks separately
    uint8_t* padded_end;
    int final_blocks_num;
    if (message_len % 512 >= 448) {
        padded_end = calloc(128, sizeof(uint8_t));
        final_blocks_num = 2;
        *(uint64_t*)(padded_end + 120) = byteSwap64(message_len);
    }
    else {
        padded_end = calloc(64, sizeof(uint8_t));
        final_blocks_num = 1;
        *(uint64_t*)(padded_end + 56) = byteSwap64(message_len);
    }

    for (int i = 0; i < (int)(message_len % 512 / 8); ++i) {
        padded_end[i] = message[(message_len / 512) * 64 + i];
    }

    if (message_len % 8 == 0) {
        padded_end[message_len % 512 / 8] = 0x80;
    }
    else {
        padded_end[message_len % 512 / 8] = message[(message_len / 512) * 64 + message_len % 512 / 8] | (1 << (8 - message_len % 8));
    }

    for (int i = 0; i < final_blocks_num; ++i) {
        compute_block(hash, padded_end + ((index + i) * 64));
    }

    free(padded_end);

    return 0;
}
