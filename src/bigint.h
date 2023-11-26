#ifndef _BIGINT_H
#define _BIGINT_H

#include <stdint.h>

#define NONNEGATIVE 0
#define NEGATIVE    1

#define DTYPE 32


#ifdef DTYPE
#if DTYPE == 8
typedef uint8_t word;
#define MAXWORD 0xff
#elif DTYPE == 32
typedef uint32_t word;
#define MAXWORD 0xffffffff
#elif DTYPE == 64
typedef uint64_t word;
#define MAXWORD 0xffffffffffffffff
#endif
#endif

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
void bi_flip_sign(bigint* x);

int get_wordlen(bigint* x);
int get_bitlen(bigint* x);
int get_ith_bit(bigint* x, int i);
int get_sign(bigint* x);
int get_filpped_sign(bigint* x);

void bi_show_hex(bigint* x);
void bi_show_hex_inorder(bigint* x);

void bi_shift_right_word(bigint** x, int size);
void bi_shift_left_word(bigint** x, int size);


void bi_shr(bigint* x, const int r);
void bi_shl(bigint** x, uint64_t r);


//todo
// void bi_show_bin(bigint* x);
// void bi_show_bin_inorder(bigint* x);

#endif
