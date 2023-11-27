#include "sha512.h"

#include <stdlib.h>

//swap from little to big endian
#define byteSwap64(x)                                                      \
	((((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | \
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) |  \
	 (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |  \
	 (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000))

static const uint64_t k[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL,
    0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL,
    0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL,
    0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL,
    0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL,
    0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
    0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

#define rotr(x, n) ((x >> n) | (x << (64 - n)))
#define shr(x, n) (x >> n)
#define ch(x, y, z) ((x & y) ^ ((~x) & z))
#define maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define Sig0(x) (rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39))
#define Sig1(x) (rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41))
#define sig0(x) (rotr(x, 1) ^ rotr(x, 8) ^ shr(x, 7))
#define sig1(x) (rotr(x, 19) ^ rotr(x, 61) ^ shr(x, 6))

static void compute_block(uint64_t* hash, const uint8_t* input) {
    uint64_t w[80];
    for (int i = 0; i < 16; ++i) {
        w[i] =
            ((uint64_t)input[0] << 56) |
            ((uint64_t)input[1] << 48) |
            ((uint64_t)input[2] << 40) |
            ((uint64_t)input[3] << 32) |
            ((uint64_t)input[4] << 24) |
            ((uint64_t)input[5] << 16) |
            ((uint64_t)input[6] << 8) |
            ((uint64_t)input[7]);
        input += 8;
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];
    }

    uint64_t a = hash[0], b = hash[1], c = hash[2], d = hash[3], e = hash[4], f = hash[5], g = hash[6], h = hash[7];
    for (int i = 0; i < 80; ++i) {
        uint64_t t1 = h + Sig1(e) + ch(e, f, g) + k[i] + w[i];
        uint64_t t2 = Sig0(a) + maj(a, b, c);
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

int sha512(uint64_t* hash, const uint8_t* message, const size_t message_len) {
    if (!hash) return 1;
    if (!message) return 2;
    if (message_len <= 0) return 3;

    hash[0] = 0x6a09e667f3bcc908ULL;
    hash[1] = 0xbb67ae8584caa73bULL;
    hash[2] = 0x3c6ef372fe94f82bULL;
    hash[3] = 0xa54ff53a5f1d36f1ULL;
    hash[4] = 0x510e527fade682d1ULL;
    hash[5] = 0x9b05688c2b3e6c1fULL;
    hash[6] = 0x1f83d9abfb41bd6bULL;
    hash[7] = 0x5be0cd19137e2179ULL;

    size_t index = 0;
    while ((index + 1) * 1024 <= message_len) {
        compute_block(hash, message + index * 128);
        ++index;
    }

    //pad and compute final blocks separately
    uint8_t* padded_end;
    int final_blocks_num;
    if (message_len % 1024 >= 896) {
        padded_end = calloc(256, sizeof(uint8_t));
        final_blocks_num = 2;
        *(uint64_t*)(padded_end + 240) = byteSwap64(message_len);
    }
    else {
        padded_end = calloc(128, sizeof(uint8_t));
        final_blocks_num = 1;
        *(uint64_t*)(padded_end + 120) = byteSwap64(message_len);
    }

    for (int i = 0; i < (int)(message_len % 1024 / 8); ++i) {
        padded_end[i] = message[(message_len / 1024) * 128 + i];
    }

    if (message_len % 8 == 0) {
        padded_end[message_len % 1024 / 8] = 0x80;
    }
    else {
        padded_end[message_len % 1024 / 8] = message[(message_len / 1024) * 128 + message_len % 1024 / 8] | (1 << (8 - message_len % 8));
    }

    for (int i = 0; i < final_blocks_num; ++i) {
        compute_block(hash, padded_end + ((index + i) * 128));
    }

    free(padded_end);

    return 0;
}
