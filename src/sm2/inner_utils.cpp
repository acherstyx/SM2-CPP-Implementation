//
// Created by acherstyx on 8/5/20.
//
#include "inner_utils.h"

#include "big.h"

#include <random>
#include <ctime>

#define BINARY_PRECISION 500

Miracl precision(BINARY_PRECISION, 2);

Big gen_SEED() {
    time_t seed;
    time(&seed);

    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);

    srand(unsigned(tn.tv_nsec));

    return rand(int(random()) % (BINARY_PRECISION - 192) + 192, 2);
}

Big Hash_256(const Big &SEED) {
    // TODO: SM3 hash
    Big mod256 = pow(Big(2), 256);
    return SEED % mod256;
}


void gen_ECC_a_b(const Big &H, const Big &p, Big &a, Big &b) {
#define R H
    Big r = R % p;
    for (int retry = 0; retry < 1000; retry++) {

    }
    throw exception();
#undef R
}


bool valid_ECC_a_b(const Big &a, const Big &b, const Big &p) {
    Big condition = (4 * a * a * a + 27 * b * b) % p;
    cout << condition;
    if (condition == 0)
        return false;
    else
        return true;
}
