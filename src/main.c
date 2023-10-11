#include "hash/sha256.h"
#include "hash/sha512.h"
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

int main(int argc, char** argv) {
    uint32_t hash[8];
    uint8_t msg[3] = { 'a', 'b', 'c' };
    sha256(hash, msg, 3 * 8);
    for (int i = 0; i < 8; ++i) {
        printf("%x", hash[i]);
    }
    printf("\n");
}