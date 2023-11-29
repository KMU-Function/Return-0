#ifndef _ARITH_H
#define _ARITH_H

#include "bigint.h"

void bi_addc(bigint** dst, bigint* x, bigint* y);
void bi_subc(bigint** dst, bigint* x, bigint* y);
void bi_add(bigint** dst, bigint* x, bigint* y);
void bi_sub(bigint** dst, bigint* x, bigint* y);

void bi_mul_singleword(word* dst, word x, word y);
void bi_mul_textbook(bigint** dst, bigint* x, bigint* y);
void bi_mul(bigint** dst, bigint* x, bigint* y, const char *mulc);
void bi_sqr_singleword(word* dst, word x);
void bi_sqr_textbook(bigint** dst, bigint* x);
void bi_sqr(bigint** dst, bigint* x, const char *mulc);
void karatsuba_mul(bigint** dest, bigint* src1, bigint* src2);

// int bi_2word_div(word* q, bigint* a, word* b);
// int bi_divcc(bigint** q, bigint** r, bigint* a, bigint* b);
void binary_DIVISION(bigint** dest_Q, bigint** dest_R, bigint* src1, bigint* src2);
int DIVISION(bigint** dest_Q, bigint** dest_R, bigint* src1, bigint* src2);
#endif
