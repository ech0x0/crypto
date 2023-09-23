#ifndef RANDOM_H
#define RANDOM_H

#include <gmp.h>
#include <stdint.h>

//returns -1 for errors otherwise returns the number of bytes read
int rand_bytes(uint8_t* bytes, size_t bytecnt);

void init_rand();
void destroy_rand();
void generate_random_number(mpz_t n, mp_bitcnt_t bitcnt);
void generate_random_prime(mpz_t prime, mp_bitcnt_t bitcnt);

#endif