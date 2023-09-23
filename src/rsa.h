#ifndef RSA_H
#define RSA_H

#include <gmp.h>

void test();
void rsa_keygen(mpz_t public_key, mpz_t private_key, mpz_t modulus, mp_bitcnt_t bitcnt);
void rsa_encrypt(mpz_t encrypted_message, mpz_t message, mpz_t public_key, mpz_t modulus);
void rsa_decrypt(mpz_t message, mpz_t encrypted_message, mpz_t private_key, mpz_t modulus);

#endif