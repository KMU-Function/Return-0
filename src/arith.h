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


int bi_2word_div(word* q, bigint* a, word* b);
int bi_divcc(bigint** q, bigint** r, bigint* a, bigint* b);

#endif
