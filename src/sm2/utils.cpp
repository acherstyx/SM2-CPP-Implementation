//
// Created by acherstyx on 8/5/20.
//
#include "utils.h"

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


void gen_ECC(const Big &H, Big &a, Big &b) {
#define R H


#undef R
}
