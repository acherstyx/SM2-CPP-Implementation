//
// Created by acherstyx on 8/5/20.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "big.h"

#include "sm2/inner_utils.h"
#include "sm2/sm2_enc.h"

TEST_CASE("Rand SEED test", "utils") {
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

TEST_CASE("Hash 256 test", "utils") {
    for (int i = 0; i < 100; i++) {
        gen_SEED();
        Big a = Hash_256(gen_SEED());
        cout << a.len() << '\n';
        REQUIRE(a.len() <= 256);
    }
}

TEST_CASE("Gen a, b for ECC", "utils") {
    for (int i = 0; i < 100; i++) {
        Big hash_seed = Hash_256(gen_SEED());
        Big p, a, b;
        p = 99997867;
        gen_ECC_a_b(hash_seed, p, a, b);
        cout << "[seed: " << hash_seed << " ";
        cout << "p: " << p << " ";
        cout << "a: " << a << " ";
        cout << "b: " << b << "]\n";
        REQUIRE(valid_ECC_a_b(a, b, p));
    }
}

TEST_CASE("Valid a, b for ECC", "utils") {
    REQUIRE(valid_ECC_a_b(-3, 2, 200) == false);
    REQUIRE(valid_ECC_a_b(-3, 2, 108) == false);
    REQUIRE(valid_ECC_a_b(3, 2, 100) == true);
    REQUIRE(valid_ECC_a_b(3, 2, 216) == false);
}

TEST_CASE("SM2 Key gen") {
    for (int i = 0; i < 10; i++) {
        Big x, y, key;
        sm2_key_gen(x, y, key);
        cout << "x: " << x << " y: " << y << " key: " << key << "\n";
//        sleep(1);
    }
}