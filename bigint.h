#ifndef _BIGINT_H
#define _BIGINT_H

#include <stdint.h>

#define NONNEGATIVE 0
#define NEGATIVE    1

typedef uint32_t word;

typedef struct _BIGINT{
    int sign ;      // NEGATIVE or NONNEGATIVE
    int wordlen ;   // wordlen >= 0
    word* a;        // address for big integer
} bigint;

void bi_delete(bigint** x);
void bi_new(bigint** x, int wordlen);

int bi_set_by_array(bigint** x, int sign, word* a, int wordlen);
int bi_set_by_string(bigint** x, int sign, char* str, int base);

void bi_refine(bigint* x);
void bi_assign(bigint** y, bigint* x);
void bi_expand(bigint* x, int new_wordlen);
void bi_expand_oneblock(bigint* x);

void bi_gen_rand(bigint** x, int sign, int wordlen);
void bi_set_one(bigint** x);
void bi_set_zero(bigint** x);
int bi_is_one(bigint* x);
int bi_is_zero(bigint* x);

int compare_abs(bigint* x, bigint* y);
int compare(bigint* x, bigint* y);

int get_wordlen(bigint* x);
int get_bitlen(bigint* x);
int get_ith_bit(bigint* x, int i);
int get_sign(bigint* x);
int get_filped_sign(bigint* x);

void bi_show_hex(bigint* x);
void bi_show_hex_inorder(bigint* x);

#endif
