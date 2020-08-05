//
// Created by acherstyx on 8/5/20.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "big.h"

#include "sm2/inner_utils.h"

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

TEST_CASE("Gen a, b for ECC") {
    for (int i = 0; i < 100; i++) {
        Big hash_seed = Hash_256(gen_SEED());
        Big p, a, b;
        p = 99997913;
        gen_ECC_a_b(hash_seed, p, a, b);
    }
}

TEST_CASE("Valid a, b for ECC") {
    REQUIRE(valid_ECC_a_b(-3, 2, 200) == false);
    REQUIRE(valid_ECC_a_b(-3, 2, 108) == false);
    REQUIRE(valid_ECC_a_b(3, 2, 100) == true);
    REQUIRE(valid_ECC_a_b(3, 2, 216) == false);
}