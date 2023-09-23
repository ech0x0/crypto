#include "rsa.h"
#include "random.h"
#include <stdint.h>

void test() {

}

void rsa_keygen(mpz_t public_key, mpz_t private_key, mpz_t modulus, mp_bitcnt_t bitcnt) {
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
