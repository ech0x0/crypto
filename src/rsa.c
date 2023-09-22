#include "rsa.h"

void generate_random_number(mpz_t n, mp_bitcnt_t bitcnt) {
    
}

void rsa_keygen(mpz_t public_key, mpz_t private_key, mpz_t modulus) {
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);

    mpz_clear(p);
    mpz_clear(q);
}

void rsa_encrypt(mpz_t encrypted_message, mpz_t message, mpz_t public_key, mpz_t modulus) {
}

void rsa_decrypt(mpz_t message, mpz_t encrypted_message, mpz_t private_key, mpz_t modulus) {
}
