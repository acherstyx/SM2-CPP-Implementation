//
// Created by acherstyx on 8/6/20.
//

#include "sm3_reference.h"
#include "sm3_miracl_wrapper.h"

#include "big.h"

#define BUFFER_MAX  10000

Big char2big(unsigned char *message, int len) {
    Big sum = 0;
    for (int i = 0; i < len; i++) {
        sum = sum * 256;
        sum += message[i];
    }
    return sum;
}

void big2char(Big message, unsigned char *ret_message, int &len) {
    int i = 0;
    unsigned char temp[BUFFER_MAX];
    while (message != 0) {
        temp[i++] = message % 256;
        message = message / 256;
    }
    len = i;
    for (i = 0; i < len; i++) {
        ret_message[i] = temp[len - i - 1];
    }
}
