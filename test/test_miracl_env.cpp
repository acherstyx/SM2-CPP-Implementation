//
// Created by acherstyx on 8/5/20.
//

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include "big.h"

extern "C" {
#include "miracl.h"
}

Miracl precision(500, 10); // This makes sure that MIRACL

int run2(int n) {
    Big nf = 1;

    while (n > 1)
        nf *= (n--);
    cout << nf << "\n";
    return 0;
}


int run1() {
    big a, b, c;
    miracl *mip = mirsys(5000, 16);
    a = mirvar(8);
    b = mirvar(7);
    c = mirvar(0);
    add(a, b, c);
    cotnum(c, stdout);
    return 0;
}

TEST_CASE("Test miracl.h function") {
    REQUIRE(run1() == 0);
}

TEST_CASE("Test big.h function") {
    REQUIRE(run2(100) == 0);
}