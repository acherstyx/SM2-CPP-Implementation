//
// Created by acherstyx on 8/7/20.
//

#ifndef SIMPLE_EXAMPLE_SM2_ENC_H
#define SIMPLE_EXAMPLE_SM2_ENC_H

#include "ecn.h"
#include "big.h"

void sm2_key_gen(Big &x, Big &y, Big &private_key);

int sm2_enc(unsigned char *msg, int msg_len, Big x, Big y, unsigned char *msg_after_enc);

#endif //SIMPLE_EXAMPLE_SM2_ENC_H
