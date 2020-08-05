//
// Created by acherstyx on 8/5/20.
//

#ifndef SIMPLE_EXAMPLE_UTILS_H
#define SIMPLE_EXAMPLE_UTILS_H

#include "big.h"


Big gen_SEED();

Big Hash_256(const Big &SEED);

void gen_ECC(const Big &H, Big &a, Big &b);

#endif //SIMPLE_EXAMPLE_UTILS_H
