//
// Created by acherstyx on 8/6/20.
//

#ifndef INNER_SM3_SM3_MIRACL_WRAPPER_H
#define INNER_SM3_SM3_MIRACL_WRAPPER_H

#include "big.h"
#include "sm3_reference.h"

Big char2big(unsigned char *message, int len);

void big2char(Big message, unsigned char *ret_message, int &len);


#endif //INNER_SM3_SM3_MIRACL_WRAPPER_H

