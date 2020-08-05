//
// Created by acherstyx on 8/5/20.
//

#ifndef SM3_SM3_H
#define SM3_SM3_H

#include <cstdio>
#include <memory.h>
/*
* SM3算法产生的哈希值大小（单位：字节）
*/
#define SM3_HASH_SIZE 32

/*
* SM3上下文
*/
typedef struct SM3Context {
    unsigned int intermediateHash[SM3_HASH_SIZE / 4];
    unsigned char messageBlock[64];
} SM3Context;

/*
* SM3计算函数
*/
unsigned char *SM3Calc(const unsigned char *message, unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]);

/*
* 判断运行环境是否为小端
*/
static const int endianTest = 1;
#define IsLittleEndian() (*(char *)&endianTest == 1)

/*
* 向左循环移位
*/
#define LeftRotate(word, bits) ( (word) << (bits) | (word) >> (32 - (bits)) )

/*
* 反转四字节整型字节序
*/
unsigned int *ReverseWord(unsigned int *word);

/*
* T
*/
unsigned int T(int i);

/*
* FF
*/
unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, int i);

/*
* GG
*/
unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, int i);

/*
* P0
*/
unsigned int P0(unsigned int X);

/*
* P1
*/
unsigned int P1(unsigned int X);

/*
* 初始化函数
*/
void SM3Init(SM3Context *context);

/*
* 处理消息块
*/
void SM3ProcessMessageBlock(SM3Context *context);

/*
* SM3算法主函数
*/
unsigned char *SM3Calc(const unsigned char *message, unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]);


#endif //SM3_SM3_H
