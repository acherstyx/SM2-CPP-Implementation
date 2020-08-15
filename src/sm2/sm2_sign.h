//
// Created by acherstyx on 8/16/20.
//

#ifndef SIMPLE_EXAMPLE_SM2_SIGN_H
#define SIMPLE_EXAMPLE_SM2_SIGN_H

#include "big.h"

#include "sm2/sm2_enc.h"
#include "sm3/sm3_miracl_wrapper.h"

Big hash_Za(unsigned char *ID_A, unsigned int ID_A_len, const Big &a, const Big &b,
            const Big &x_g, const Big &y_g, const Big &x_a, const Big &y_a);

void sm2_sign(unsigned char *ID_A, unsigned int ID_A_len, unsigned char *message, unsigned int message_len,
              const Big &key, Big &r, Big &s);

bool sm2_verify(unsigned char *ID_A, unsigned int ID_A_len, unsigned char *message, unsigned int message_len,
                const Big &r, const Big &s, const Big &xa, const Big &ya);

#endif //SIMPLE_EXAMPLE_SM2_SIGN_H
