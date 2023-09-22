#ifndef RSA_C
#define RSA_C

#include <gmp.h>

void rsa_keygen(mpz_t public_key, mpz_t private_key, mpz_t modulus);
void rsa_encrypt(mpz_t encrypted_message, mpz_t message, mpz_t public_key, mpz_t modulus);
void rsa_decrypt(mpz_t message, mpz_t encrypted_message, mpz_t private_key, mpz_t modulus);

#endif