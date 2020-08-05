//
// Created by acherstyx on 8/5/20.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "big.h"

#include "sm2/utils.h"

TEST_CASE("Rand SEED test") {
    for (int i = 0; i < 100; i++) {
        Big a = gen_SEED();
        Big b = gen_SEED();
        cout << "case " << i << '\n';
        cout << "a: " << a << '\n';
        cout << "b: " << b << '\n';
        REQUIRE(a != b);
        REQUIRE(a.len() >= 192);
        REQUIRE(b.len() >= 192);
    }
}

TEST_CASE("Hash 256 test") {
    for (int i = 0; i < 100; i++) {
        gen_SEED();
        Big a = Hash_256(gen_SEED());
        cout << a.len() << '\n';
        REQUIRE(a.len() <= 256);
    }
}