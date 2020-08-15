//
// Created by acherstyx on 8/7/20.
//

#ifndef SIMPLE_EXAMPLE_SM2_ENC_H
#define SIMPLE_EXAMPLE_SM2_ENC_H

#include "ecn.h"
#include "big.h"
#include "sm3.h"

typedef struct FPECC {
    char *p;
    char *a;
    char *b;
    char *n;
    char *x;
    char *y;
} FPECC;

/*SM2*/
extern struct FPECC Ecc256;

void sm2_key_gen(Big &x, Big &y, Big &private_key);

int sm2_enc(unsigned char *msg, int msg_len, Big x, Big y, unsigned char *msg_after_enc);

int sm2_dec(unsigned char *msg, int msg_len, Big d, unsigned char *msg_dec);

#endif //SIMPLE_EXAMPLE_SM2_ENC_H
