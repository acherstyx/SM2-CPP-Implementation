//
// Created by acherstyx on 8/6/20.
//

#ifndef INNER_SM3_SM3_MIRACL_WRAPPER_H
#define INNER_SM3_SM3_MIRACL_WRAPPER_H

#include "big.h"
#include "sm3_reference.h"

// transfer between two data format
Big char2big(unsigned char *message, unsigned int len);

void big2char(Big message, unsigned char *ret_message, unsigned int &len);

Big SM3_Hash_256(const Big &message);

#endif //INNER_SM3_SM3_MIRACL_WRAPPER_H

