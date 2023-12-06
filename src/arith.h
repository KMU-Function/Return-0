/**
 * @file arith.h
 * @brief Header file for big integer arithmetic operations.
 */

#ifndef _ARITH_H
#define _ARITH_H

#include "bigint.h"

word bi_addcc(word* dst, word x, word y, word c);
int bi_addc(bigint** dst, bigint* x, bigint* y);
int bi_add(bigint** dst, bigint* x, bigint* y);

word bi_subcc(word* dst, word x, word y, word b);
int bi_subc(bigint** dst, bigint* x, bigint* y);
int bi_sub(bigint** dst, bigint* x, bigint* y);

void bi_mulc(word* dst, word x, word y);
int bi_mul_textbook(bigint** dst, bigint* x, bigint* y);
void bi_mul_improvedtextbook(bigint** dst, bigint* x, bigint* y);
void bi_mul_karatsuba(bigint** dst, bigint* x, bigint* y);
void bi_mul_karatsuba_core(bigint** dst, bigint* x, bigint* y, uint64_t len);
void bi_mul(bigint** dst, bigint* x, bigint* y, const char* mulc);

void bi_sqrc(word* dst, word x);
void bi_sqr_textbook(bigint** dst, bigint* x);
void bi_sqr(bigint** dst, bigint* x, const char* mulc);

int bi_binary_longdiv(bigint** q, bigint** r, bigint* a, bigint* b);

void bi_barrett_compute_t(bigint** t, bigint* y);
int bi_barrett_reduction_core(bigint** r, bigint* x, bigint* m, bigint* t);
int bi_barrett_reduction(bigint** r, bigint* x, bigint* m);


void bi_LtR(bigint** dst, bigint** x, bigint* n);
void bi_LtR_mod(bigint** dst, bigint** x, bigint* n, bigint* modulo);

#endif
