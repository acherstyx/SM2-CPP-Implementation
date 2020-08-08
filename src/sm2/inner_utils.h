//
// Created by acherstyx on 8/5/20.
//

#ifndef SIMPLE_EXAMPLE_INNER_UTILS_H
#define SIMPLE_EXAMPLE_INNER_UTILS_H

#include "big.h"
#include "sm3.h"

Big gen_SEED();

Big Hash_256(const Big &SEED);

void gen_ECC_a_b(const Big &H, const Big &p, Big &a, Big &b);

bool valid_ECC_a_b(const Big &a, const Big &b, const Big &p);

#endif //SIMPLE_EXAMPLE_INNER_UTILS_H
