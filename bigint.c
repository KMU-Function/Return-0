#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "bigint.h"
#include "array.h"

//todo shift, reduction, show_bin/dec


void bi_delete(bigint** x){
    if(*x == NULL)
        return;

#ifdef ZEROIZE 
    init_array((*x)->a, (*x)->wordlen);
#endif

    free((*x)->a);
    free(*x);
    *x = NULL;
}

/**
* @brief Create BigInt x
*/
void bi_new(bigint** x, int wordlen) {
    if(*x != NULL)
        bi_delete(x);

    *x = (bigint*)calloc(1, sizeof(bigint));
    (*x)->sign = NONNEGATIVE;
    (*x)->wordlen = wordlen;
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}

/**
* @brief Create BigInt x by array
*/
int bi_set_by_array(bigint** x, int sign, word* a, int wordlen) {
    (*x)->sign = sign;
    (*x)->wordlen = wordlen;
    for(int i = 0; i < wordlen; i++){
        (*x)->a[i] = a[i];
    }
    return 0;
}

//todo
/**
* @brief Create BigInt x by string
*/
int bi_set_by_string(bigint** x, int sign, char* str, int base) {
    return 0;
}

/**
* @brief Refine BigInt x (Remove Last Zero Words)
*/
void bi_refine(bigint* x) {
    if(x == NULL)
        return;

    int new_wordlen = x->wordlen;

    while (new_wordlen > 1) {
        if(x->a[new_wordlen - 1] != 0)
            break;
        new_wordlen--;
    }
    if(x->wordlen != new_wordlen){
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    }

    if((x->wordlen == 1) && (x->a[0] == 0x00))
        x->sign = NONNEGATIVE;
}

/**
* @brief Assign BigInt x to y (Y <- X)
*/
void bi_assign(bigint** y, bigint* x){
    if(*y != NULL)
        bi_delete(y);
    
    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    copy_array((*y)->a, x->a, x->wordlen);
}

/**
* @brief Expand BigInt x with new word length by zero
*/
void bi_expand(bigint* x, int new_wordlen){
    if(x->wordlen >= new_wordlen){
        fprintf(stderr, "wordlen is larger or equal to new wordlen\n");
        return;
    }

    int old_wordlen = x->wordlen;
    x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    x->wordlen = new_wordlen;

    // initialize reallocates memory into zero
    for(int i = old_wordlen; i < x->wordlen; i++){
        x->a[i] = 0;
    }
}

/**
* @brief Generate random BigInt x
*/
void bi_gen_rand(bigint** x, int sign, int wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    gen_rand_array((*x)->a, (*x)->wordlen);     // get random data
    bi_refine(*x);
}

/**
* @brief Create BigInt x and initialize its value to 0x1
*/
void bi_set_one(bigint** x) {
    bi_new(x, 1);
    (*x)->sign = NONNEGATIVE;
    (*x)->a[0] = 0x1;
}

/**
* @brief Create BigInt x and initialize its value to 0x0
*/
void bi_set_zero(bigint** x) {
    bi_new(x, 1);
    (*x)->sign = NONNEGATIVE;
    (*x)->a[0] = 0x0;
}

/**
* @brief Check if value of BigInt x is 0x1
* return 1(true) if BigInt x is 0x1, 0(false) otherwise
*/
int bi_is_one(bigint* x) {
    if(x->sign != NONNEGATIVE || x->a[0] != 1)
        return false;
    for(int i = 0; i < x->wordlen - 1; i++){
        if(x->a[i] != 0)
            return false;
    }
    return true;
}

/**
* @brief Check if value of BigInt x is 0x0
* return 1(true) if BigInt x is 0x0, 0(false) otherwise
*/
int bi_is_zero(bigint* x) {
    if(x->sign != NONNEGATIVE || x->a[0] != 0)
        return false;
    for(int i = 0; i < x->wordlen - 1; i++){
        if(x->a[i] != 0)
            return false;
    }
    return true;
}

/**
* @brief Compare absolute value of positive BigInt x and y
* return 1 if x > y
* return 0 if x = y
* return -1 if x < y
*/
int compare_abs(bigint* x, bigint* y){
    if(x->wordlen > y->wordlen)
        return 1;
    else if (x-> wordlen < y->wordlen)
        return -1;
    for(int i = x->wordlen - 1; i >= 0; i++){
        if(x->a[i] > y->a[i])
            return 1;
        else if(x->a[i] < y->a[i])
            return -1;
    }
    return 0;
}

/**
* @brief Flip sign bit of BigInt x
*/
void bi_flip_sign(bigint* x) {
    if(x->sign == NONNEGATIVE){
        x->sign = NEGATIVE;
    }
    else{
        x->sign = NONNEGATIVE;
    }
}

/**
* @brief Compare value of BigInt x and y
* return 1 if x > y
* return 0 if x = y
* return -1 if x < y
*/
int compare(bigint* x, bigint* y){
    if(x->sign == NONNEGATIVE && y->sign == NEGATIVE)
        return 1;
    if(x->sign == NEGATIVE && y->sign == NONNEGATIVE)
        return -1;
    
    int ret = compare_abs(x, y);
    if(x->sign == NONNEGATIVE) 
        return ret;
    else
        return -ret;
}

/**
* @brief Get word length of BigInt x
*/
int get_wordlen(bigint* x) {
    return x->wordlen;
}

/**
* @brief Get bit length of BigInt x
*/
int get_bitlen(bigint* x) {
    return x->wordlen * sizeof(word) * 8;
}

/**
* @brief Get i-th bit of BigInt x
* @param i index of the bit to be extracted
*/
int get_ith_bit(bigint* x, int i) {
    return (x->a[i>>3] >> (i - (i>>3))) & 0x1;
}

/**
* @brief Get sign bit of BigInt x
*/
int get_sign(bigint* x) {
    return x->sign;
}

/**
* @brief Get flipped sign bit of BigInt x
*/
int get_filpped_sign(bigint* x) {
    if(x->sign == NONNEGATIVE)
        return NEGATIVE;
    else
        return NONNEGATIVE;
}

/**
* @brief Print BigInt x in hexa representation
*/
void bi_show_hex(bigint* x){
    for(int i = 0; i < x->wordlen; i++){
#if DTYPE == 8
        printf("%02x ", x->a[i]);
#elif DTYPE == 32
        printf("%08x ", x->a[i]);
#elif DTYPE == 64
        printf("%016x ", x->a[i]);
#else
#error "UNSUPPORTED TYPE\n";
#endif
    }printf("\n");
}

/**
* @brief Print BigInt x in hexa representation in order
*/
void bi_show_hex_inorder(bigint* x) {
    for(int i = x->wordlen - 1; i >= 0; i--){
        printf("%08x ", x->a[i]);
    }printf("\n");
}

//todo
/**
* @brief Print BigInt x in binary representation in order
*/
// void bi_show_bin(bigint* x){}
// void bi_show_bin_inorder(bigint* x){}
