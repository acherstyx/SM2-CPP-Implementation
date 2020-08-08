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
    Big x, y, key;

    sm2_key_gen(x, y, key);
    cout << "x: " << x << " y: " << y << " key: " << key << "\n";
}

TEST_CASE("SM2 encrypt and decrypt") {
    Big x, y, key;
    unsigned char msg[50] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    unsigned char msg_dec[50] = {0};
    int klen = 19;
    int enc_klen;
    unsigned char out[1000];

    sm2_key_gen(x, y, key);

    enc_klen = sm2_enc(msg, klen, x, y, out);
    cout << "message after encrypt: " << out << "\n";
    REQUIRE(enc_klen == klen + 96);

    int ret = sm2_dec(out, enc_klen, key, msg_dec);

    cout << "dec: ";
    for (int i = 0; i < ret; i++)
        cout << int(msg_dec[i]) << " ";
    cout << "\n";
}