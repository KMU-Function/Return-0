#ifndef _ARITH_H
#define _ARITH_H

#include "bigint.h"

int bi_add(bigint **z, bigint *x, bigint *y);
int bi_sub(bigint **z, bigint *x, bigint *y);

int bi_mul_textbook(bigint** dst, bigint* x, bigint* y);
void bi_mul(bigint** dst, bigint* x, bigint* y, const char *mulc);
void bi_sqr_textbook(bigint** dst, bigint* x);
void bi_sqr(bigint** dst, bigint* x, const char *mulc);
void karatsuba_mul(bigint** dest, bigint* src1, bigint* src2);



int bi_binary_longdiv(bigint** q, bigint** r, bigint* a, bigint* b);
void bi_compute_barrett_t(bigint** t, bigint* x);
int bi_barrett_reduction(bigint** r, bigint* x, bigint* m, bigint* t);

int bi_div(bigint **q, bigint **r, const bigint *x, const bigint *y);

#endif
