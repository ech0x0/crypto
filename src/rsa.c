#include "rsa.h"
#include "random.h"

void rsa_keygen(mpz_t e, mpz_t d, mpz_t n, mp_bitcnt_t bitcnt) {
    init_rand();

    uint8_t ok = 0;
    while (ok == 0) {
        ok = 1;
        mpz_t p, q;
        mpz_init(p);
        mpz_init(q);

        //get 2 random prime numbers
        generate_random_prime(p, bitcnt / 2);
        generate_random_prime(q, bitcnt / 2);

        //calculate n = modulus
        mpz_mul(n, p, q);

        //calculate phi = (p - 1)(q - 1)
        mpz_t phi;
        mpz_init(phi);

        mpz_t p1, q1;
        mpz_init(p1);
        mpz_init(q1);

        mpz_sub_ui(p1, p, 1);
        mpz_sub_ui(q1, q, 1);

        mpz_mul(phi, p1, q1);

        mpz_clear(p1);
        mpz_clear(q1);

        //e = 65537
        //e and n form the public key
        mpz_set_str(e, "65537", 10);

        //check if gcd(e, phi) = 1, if not find new primes and recalculate it
        mpz_t gcd_e_phi;
        mpz_init(gcd_e_phi);

        mpz_gcd(gcd_e_phi, e, phi);
        if (mpz_cmp_ui(gcd_e_phi, 1) != 0) ok = 0;

        mpz_clear(gcd_e_phi);

        //calculate d 
        //d and n form the private key
        // extended euclid calculates gcd and x and y such that ax + by = gcd(a, b)
        // if we put in e and phi we get ex + phi*y = gcd(e, phi)
        // but gcd(e, phi) = 1 so  ex + phi*y = 1
        // re arrange: ex = -phi*y + 1
        // this is equivalent to: ex % phi = 1
        // which means d = x
        //we don't need gcd, and y so we can just put NULL instead
        mpz_gcdext(NULL, d, NULL, e, phi);

        //if d is negative add phi to it to make it positive
        // because ed % phi = 1 adding or subtracting phi to d makes the equation have the same result
        if (mpz_cmp_ui(d, 0) <= 0) {
            mpz_mod(d, d, phi);
            mpz_add(d, d, phi);
        }

        mpz_clear(phi);
        mpz_clear(p);
        mpz_clear(q);
    }

    destroy_rand();
}

void rsa_encrypt(mpz_t encrypted_message, mpz_t message, mpz_t e, mpz_t n) {
    //enc_m = (m ^ e) % n
    mpz_powm(encrypted_message, message, e, n);
}

void rsa_decrypt(mpz_t message, mpz_t encrypted_message, mpz_t d, mpz_t n) {
    //m = (enc_m ^ d) % n;
    mpz_powm(message, encrypted_message, d, n);
}
