//
// Created by acherstyx on 8/6/20.
//

#define CATCH_CONFIG_MAIN

#include <cstdio>
#include <catch2/catch.hpp>

#include <sm3/sm3_reference.h>
#include <sm3/sm3_miracl_wrapper.h>

using namespace std;

TEST_CASE("SM3 hash test") {
    unsigned char words[100] = "123456kjhkjh7890";
    unsigned char message[1000] = {0};
    SM3Calc(words, 10, message);
    for (int i = 0; i < 100; i++) {
        if (i == 32) {
            cout << "||";
        }
        cout << '[' << int(message[i]) << ']';
    }
}

TEST_CASE("Transfer between [unsigned char] and [Big]") {
    SECTION("unsigned char to Big") {
        unsigned char message[100] = {2, 0};
        REQUIRE(char2big(message, 2) == 512);
    }

    SECTION("Big->char->Big") {
        for (int i = 0; i < 100; i++) { // random test case
            unsigned char message[1000];
            Big a = rand(200, 2);
            Big b;
            int len;
            cout << a << '\n';
            big2char(a, message, len);

            for (int ii = 0; ii < 100; ii++) {
                if (ii == 32) {
                    cout << "||";
                }
                cout << '[' << int(message[ii]) << ']';
            }
            cout << "\n";

            b = char2big(message, len);
            REQUIRE(a == b);
        }
    }
}