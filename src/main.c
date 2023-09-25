#include "hash/sha256.h"
#include <stdio.h>

int main(int argc, char** argv) {
    uint8_t message[3] = { 0x61, 0x62, 0x63 };
    uint32_t hash[8];
    sha256(hash, message, 3 * 8);
    for (int i = 0; i < 8; ++i) {
        printf("%x\n", hash[i]);
    }
}