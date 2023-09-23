#ifndef RSA_H
#define RSA_H

#include <gmp.h>

void rsa_keygen(mpz_t e, mpz_t d, mpz_t n, mp_bitcnt_t bitcnt);
void rsa_encrypt(mpz_t encrypted_message, mpz_t message, mpz_t e, mpz_t n);
void rsa_decrypt(mpz_t message, mpz_t encrypted_message, mpz_t d, mpz_t n);

#endif