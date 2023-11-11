#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "bigint.h"

//todo addc, multi-precision add, final add, sub

/**
* @brief Calculate addition of same signed two BigInt x, y
* Assume x, y != 0
* @param dst pointer of destination where the result of x + y will be stored
*/
void bi_addc(bigint** dst, bigint* x, bigint* y){
    int carry = 0;

    // expand smaller one
    if(x->wordlen < y->wordlen){
        bi_expand(x, y->wordlen);
    } 
    else if(y->wordlen < x->wordlen){
        bi_expand(y, x->wordlen);
    }

    // if dst->wordlen is smaller than x or y, then realloc dst->wordlen
    // check for only x because now x->wordlen and y->wordlen are the same
    if((*dst)->wordlen <= x->wordlen){
        bi_expand(*dst, x->wordlen + 1);
    }

    for(int i = 0; i < x->wordlen; i++){
        (*dst)->a[i] = x->a[i] + y->a[i] + carry;
        // check carry
        if ((*dst)->a[i] == x->a[i]) {
            continue;
        }
        if ((*dst)->a[i] < x->a[i]) {
            carry = 1;
        }
        else {
            carry = 0;
        }
    }

    if(carry == 1){
        (*dst)->a[(*dst)->wordlen - 1] = 1;
    }

    // remove zero words
    bi_refine(*dst);
}

/**
* @brief Calculate subtraction of two positive BigInt x, y 
* Assume 0 < y <= x
* @param dst pointer of destination where the result of x - y will be stored
*/
void bi_subc(bigint** dst, bigint* x, bigint* y){
    int borrow = 0;

    if(y->wordlen < x->wordlen){
        bi_new(dst, x->wordlen);
    }
    else {
        bi_new(dst, y->wordlen);
    }

    // Expand y if y->wordlen is less than x->wordlen
    if(y->wordlen < x->wordlen){
        bi_expand(y, x->wordlen);
    }

    for(int i = 0; i < x->wordlen; i++){
        (*dst)->a[i] = x->a[i] - y->a[i] - borrow;

        // check borrow
        if(x->a[i] < y->a[i] || ((x->a[i] == y->a[i]) && (borrow == 1))){
            borrow = 1;
        }
        else{
            borrow = 0;
        }
    }

    bi_refine(*dst);
}

/**
* @brief Calculate addition of two BigInt x, y
* @param dst pointer of destination where the result of x - y will be stored
*/
// 부호에 상관 없다는 내용을 넣을까 말까
void bi_add(bigint** dst, bigint* x, bigint* y) {
#ifdef ZERO_OPTIMIZE
    // check whether x == 0 or y == 0
    if(bi_is_zero(x)){
        bi_assign(&dst, y);
        return;
    }
    if(bi_is_zero(y)){
        bi_assign(&dst, x);
        return;
    }
#endif
    
    if((x->sign == NONNEGATIVE) && (y->sign == NEGATIVE)){
        // if x >= y, then dst <- x - y (non-negative)
        // if x < y, then dst <- y - x (negative because y is negative)
        if(compare_abs(x, y) >= 0){
            bi_subc(dst, x, y);
            (*dst)->sign = NONNEGATIVE;
        }
        else{
            bi_subc(dst, y, x);
            (*dst)->sign = NEGATIVE;
        }
    }
    else if((x->sign == NEGATIVE) && (y->sign == NONNEGATIVE)){
        // if x >= y, then dst <- x - y (negative because x is negative)
        // if x < y, then dst <- y - x (non-negative)
        if(compare_abs(x, y) >= 0){
            bi_subc(dst, x, y);
            (*dst)->sign = NEGATIVE;
        }
        else{
            bi_subc(dst, y, x);
            (*dst)->sign = NONNEGATIVE;
        }
    }
    else if(x->wordlen >= y->wordlen){
        bi_addc(dst, x, y);
    }
    else{
        bi_addc(dst, y, x);
    }

    return;
}

// Big Number Subtraction
// Performs dst <- x - y independent to the sign of x and y.
void bi_sub(bigint** dst, bigint* x, bigint* y){
#ifdef ZERO_OPTIMIZE
    // if x == 0, then return -y
    if(bi_is_zero(x)){
        bi_assign(dst, y);
        bi_flip_sign(*dst);
        return;
    }
    // if y == 0, then return -x
    if(bi_is_zero(y)){
        bi_assign(dst, x);
        bi_flip_sign(*dst);
        return;
    }  
    // if x == y, then return 0
    if(compare(x, y) == 0){
        bi_set_zero(*dst);
        return;
    }
#endif

    if((y->sign == NONNEGATIVE) && (compare(x, y) >= 0)){
        bi_subc(dst, x, y);
    }
    else if((x->sign == NONNEGATIVE) && (compare(y, x) >= 0)){
        bi_subc(dst, y, x);
    }
    else if ((x->sign == NEGATIVE) && (compare(x, y) >= 0)){
        bi_subc(dst, y, x);
        (*dst)->sign = NONNEGATIVE;
    }
    else if ((y->sign == NEGATIVE) && (compare(y, x) >= 0)){
        bi_subc(dst, x, y);
        (*dst)->sign = NEGATIVE;
    }
    else if ((x->sign == NONNEGATIVE) && (y->sign = NEGATIVE)){
        bi_addc(dst, x, y);
        (*dst)->sign = NONNEGATIVE;
    }
    else if ((y->sign == NONNEGATIVE) && (x->sign = NEGATIVE)){
        bi_addc(dst, x, y);
        (*dst)->sign = NEGATIVE;
    }
    return;
}