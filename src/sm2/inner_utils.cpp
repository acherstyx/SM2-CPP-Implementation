//
// Created by acherstyx on 8/5/20.
//
#include <iostream>
#include <ctime>
#include "inner_utils.h"
#include "sm3.h"
#include "ecn.h"

#include "big.h"

extern "C" {
#include "miracl.h"
}

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
    return SM3_Hash_256(SEED);
}


void gen_ECC_a_b(const Big &H, const Big &p, Big &a_, Big &b_) {
#define R H
    while (true) {
        Big r = R % p;

        b_ = r;
        a_ = rand(p.len() + 10, 2) % p;
        if (valid_ECC_a_b(a_, b_, p))
            return;
        else
            continue;
    }

#undef R
}


bool valid_ECC_a_b(const Big &a, const Big &b, const Big &p) {
    Big condition = (4 * a * a * a + 27 * b * b) % p;
//    cout << condition;
    if (condition == 0)
        return false;
    else
        return true;
}


