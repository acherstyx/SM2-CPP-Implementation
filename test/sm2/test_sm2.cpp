//
// Created by acherstyx on 8/5/20.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "big.h"

#include "precision.h"
#include "sm2/inner_utils.h"
#include "sm2/sm2_enc.h"


TEST_CASE("Utils") {
    SECTION("Rand SEED test", "utils") {
        for (int i = 0; i < 100; i++) {
            Big a = gen_SEED();
            Big b = gen_SEED();
            cout << "case " << i << '\n';
            cout << "[a]: " << a.len() << " " << a << '\n';
            cout << "[b]: " << b.len() << " " << b << '\n';
//            REQUIRE(a != b);
//            REQUIRE(a.len() >= 192);
//            REQUIRE(b.len() >= 192);
        }
    }

    SECTION("Hash 256 test", "utils") {
        for (int i = 0; i < 100; i++) {
            gen_SEED();
            Big a = Hash_256(gen_SEED());
            cout << a.len() << '\n';
            REQUIRE(a.len() <= 256);
        }
    }

    SECTION("Gen a, b for ECC", "utils") {
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

    SECTION("Valid a, b for ECC", "utils") {
        REQUIRE(valid_ECC_a_b(-3, 2, 200) == false);
        REQUIRE(valid_ECC_a_b(-3, 2, 108) == false);
        REQUIRE(valid_ECC_a_b(3, 2, 100) == true);
        REQUIRE(valid_ECC_a_b(3, 2, 216) == false);
    }
}

TEST_CASE("SM2 encryption") {

    SECTION("SM2 Key gen") {
        Big x, y, key;

        sm2_key_gen(x, y, key);
        cout << "x: " << x << " y: " << y << " key: " << key << "\n";
    }

    SECTION("SM2 encrypt and decrypt") {
        Big x, y, key;
        unsigned char msg[50] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
        unsigned char msg_dec[50] = {0};
        int klen = 19;
        int enc_klen;
        unsigned char out[1000];

        sm2_key_gen(x, y, key);

        cout << "key generation:" << "\n";
        cout << "\t" << "x: " << x << " y: " << y << "\n";
        cout << "\t" << "private key(d): " << key << "\n";

        cout << "message before encrypt: ";
        for (int i = 0; i < klen; i++)
            cout << int(msg[i]) << " ";
        cout << "\n";

        enc_klen = sm2_enc(msg, klen, x, y, out);

        cout << "message after encrypt: ";
        for (int i = 0; i < enc_klen; i++) {
            cout << int(out[i]) << " ";
        };
        cout << "\n";

        REQUIRE(enc_klen == klen + 96);

        int ret = sm2_dec(out, enc_klen, key, msg_dec);

        cout << "decrypt result: ";
        for (int i = 0; i < ret; i++)
            cout << int(msg_dec[i]) << " ";
        cout << "\n";

        REQUIRE(ret == klen);

        for (int i = 0; i < klen; i++) {
            REQUIRE(msg[i] == msg_dec[i]);
        }
    }
}