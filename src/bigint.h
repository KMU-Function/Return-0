/**
 * @file bigint.h
 * @brief Header file for big integer basic operations.
 */

#ifndef _BIGINT_H
#define _BIGINT_H

#include <stdint.h>

#define NONNEGATIVE 0
#define NEGATIVE    1

 // #define DTYPE 32


#ifdef DTYPE
#if DTYPE == 8
typedef uint8_t word;
#define MAXWORD UINT8_MAX
#elif DTYPE == 32
typedef uint32_t word;
#define MAXWORD UINT32_MAX
#elif DTYPE == 64
typedef unsigned long long word;
#define MAXWORD UINT64_MAX
#endif
#endif

typedef struct _BIGINT {
    int sign;      // NEGATIVE or NONNEGATIVE
    int wordlen;   // wordlen >= 0
    word* a;        // address for big integer
} bigint;

void bi_delete(bigint** x);
void bi_new(bigint** x, int wordlen);
void bi_refine(bigint* x);
void bi_assign(bigint** y, bigint* x);
void bi_expand(bigint* x, int new_wordlen);

void bi_gen_rand(bigint** x, int sign, int wordlen);
void bi_set_one(bigint** x);
void bi_set_zero(bigint** x);
void bi_flip_sign(bigint* x);
int bi_set_by_array(bigint** x, int sign, word* a, int wordlen);
void bi_set_min_words(bigint** x, int sign, size_t wordlen);

int bi_is_one(bigint* x);
int bi_is_zero(bigint* x);

int compare(bigint* x, bigint* y);
int compare_abs(bigint* x, bigint* y);

int get_wordlen(bigint* x);
int get_bitlen(bigint* x);
int get_ith_bit(bigint* x, int i);
int get_sign(bigint* x);
int get_filpped_sign(bigint* x);

void bi_show_hex(bigint* x);
void bi_show_hex_inorder(bigint* x);

void bi_shr(bigint** x, int r);
void bi_shl(bigint** x, int r);
void bi_shift_right_word(bigint** x, int bytelen);
void bi_shift_left_word(bigint** x, int bytelen);


//todo
// void bi_show_bin(bigint* x);
// void bi_show_bin_inorder(bigint* x);

#endif
