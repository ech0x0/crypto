#include "hash/sha256.h"
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

int main(int argc, char** argv) {
    double time = 0;
    for (int j = 0; j < 100; ++j) {
        clock_t b = clock();
        for (int i = 0; i < 10000; ++i) {
            uint8_t message[4] = { 0x61, 0x62, 0x63, (uint8_t)(i % 255) };
            uint32_t hash[8];
            sha256(hash, message, 4 * 8);
        }
        clock_t e = clock();
        time += (double)(e - b) / CLOCKS_PER_SEC;
    }
    printf("%f\n", time);
}