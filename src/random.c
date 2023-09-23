#include "random.h"
#include "get_time.h"

#include <unistd.h>
#include <fcntl.h>


int rand_bytes(uint8_t* bytes, size_t bytecnt) {
    int file = open("/dev/urandom", O_RDONLY);
    if (file == -1) return -1;
    return read(file, bytes, bytecnt);
}

gmp_randstate_t state;
mpz_t seed;

void init_rand() {
    mpz_init(seed);
    mpz_set_ui(seed, system_current_time_millis());

    gmp_randinit_mt(state);
    gmp_randseed(state, seed);
}

void destroy_rand() {
    gmp_randclear(state);
    mpz_clear(seed);
}

void generate_random_number(mpz_t n, mp_bitcnt_t bitcnt) {
    while (mpz_sizeinbase(n, 2) != bitcnt) mpz_urandomb(n, state, bitcnt);
}

void generate_random_prime(mpz_t prime, mp_bitcnt_t bitcnt) {
    mpz_t n;
    mpz_init(n);

    generate_random_number(n, bitcnt);
    mpz_nextprime(prime, n);
    while (mpz_probab_prime_p(prime, 100) < 1) {
        mpz_set(n, prime);
        mpz_nextprime(prime, n);
    }

    mpz_clear(n);
}