#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "bigint.h"

//todo addc, multi-precision add, final add, sub

/**
* @brief Calculate addition of same signed two BigInt x, y
* Assume x, y != 0
* @param dst pointer of destination where the result of x + y will be stored
*/
void bi_addc(bigint** dst, bigint* x, bigint* y){

    if ((x->sign ^ y->sign) != 0) {
        fprintf(stderr, "bi_addc input value's sign is not same\n");
        return;
    }

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
void bi_subc(bigint** dst, bigint* x, bigint* y) {

    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        fprintf(stderr, "bi_subc input value is NEGATIVE\n");
        return;
    }

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

    if ((y->sign == NONNEGATIVE) && (compare(x, y) >= 0)) {
        // 0 < y <= x
        bi_subc(dst, x, y);
        (*dst)->sign = NONNEGATIVE;
    }
    else if ((x->sign == NONNEGATIVE) && (compare(y, x) >= 0)) {
        // 0 < x <= y
        bi_subc(dst, y, x);
        (*dst)->sign = NEGATIVE;
    }
    else if ((x->sign == NEGATIVE) && (compare(x, y) >= 0)) {
        // y <= x < 0
        bi_subc(dst, y, x);
        (*dst)->sign = NONNEGATIVE;
    }
    else if ((y->sign == NEGATIVE) && (compare(y, x) >= 0)) {
        // x <= y < 0
        bi_subc(dst, x, y);
        (*dst)->sign = NEGATIVE;
    }
    else if ((x->sign == NONNEGATIVE) && (y->sign == NEGATIVE)) {
        // y < 0 < x
        bi_addc(dst, x, y);     // 이거 add로 할 필요 없겠지?
        (*dst)->sign = NONNEGATIVE;
    }
    else if ((y->sign == NONNEGATIVE) && (x->sign == NEGATIVE)) {
        // x < 0 < y
        bi_addc(dst, x, y);
        (*dst)->sign = NEGATIVE;
    }

    return;
}

void bi_mul_singleword(word* dst, word x, word y) {
    word x0 = x & ((1 << (sizeof(word) * 8 / 2)) - 1);
    word x1 = x >> (sizeof(word) * 8 / 2);

    word y0 = y & ((1 << (sizeof(word) * 8 / 2)) - 1);
    word y1 = y >> (sizeof(word) * 8 / 2);

    word t, t0, t1, c0, c1;

    t0 = x0 * y1;
    t1 = x1 * y0;
    t0 = t0 + t1;
    t1 = (t0 < t1) ? 1 : 0;

    c0 = x0 * y0;
    c1 = x1 * y1;
    t = c0;
    // printf("%08X\n", t0);
    // printf("%08X\n", (t0 << (sizeof(word) * 8 / 2)));
    c0 = c0 + (t0 << (sizeof(word) * 8 / 2));
    c1 = c1 + (t1 << (sizeof(word) * 8 / 2)) + (t0 >> (sizeof(word) * 8 / 2)) + (c0 < t ? 1 : 0);

    dst[1] = c1;
    dst[0] = c0;

    return;
}

void bi_mul_textbook(bigint** dst, bigint* x, bigint* y) {

    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        fprintf(stderr, "Textbook multiplication input value is NEGATIVE \n");
        return;
    }

    bigint *c = NULL;
    bigint *t = NULL;

    bi_new(&c, x->wordlen + y->wordlen);
    bi_new(&t, 2);

    for (int j = 0; j < x->wordlen; j++) {
        for (int i = 0; i < y->wordlen; i++) {
            bi_mul_singleword(t->a, x->a[j], y->a[i]);
            // printf("i + j = %d + %d = %d\n", i, j, i + j);
            bi_shift_left_word(&t, i + j);
            // printf("t = \n");
            // bi_show_hex_inorder(t);
            // printf("dst = \n");
            // bi_show_hex_inorder(*dst);
            // printf("\n");
            bi_assign(&c, *dst);    // 불필요한 복사같은데 어떻게 수정할까?
            bi_addc(dst, c, t);
            bi_new(&t, 2);
        }
    }

    // printf("dst: %d\n", (*dst)->wordlen);
    bi_delete(&c);
    bi_delete(&t);

    return;
}

void bi_mul(bigint** dst, bigint* x, bigint* y, const char *mulc) {
    if (bi_is_zero(x) || bi_is_zero(y)) {
        bi_set_zero(dst);
        return;
    }

    if (bi_is_one(x)) {
        bi_assign(dst, y);
        return;
    }

    if (bi_is_one(y)) {
        bi_assign(dst, x);
        return;
    }

    bigint* tmp = NULL;
    // x is -1
    bi_assign(&tmp, x);
    bi_flip_sign(tmp);  
    if (bi_is_one(tmp)) {
        bi_assign(dst, y);
        bi_flip_sign(*dst);
        bi_delete(&tmp);
        return;
    }

    // y is -1
    bi_assign(&tmp, y);
    bi_flip_sign(tmp);  
    if (bi_is_one(tmp)) {
        bi_assign(dst, x);
        bi_flip_sign(*dst);
        bi_delete(&tmp);
        return;
    }

    bi_delete(&tmp);

    bigint* x_tmp = NULL;
    bigint* y_tmp = NULL;
    bi_assign(&x_tmp, x);
    bi_assign(&y_tmp, y);
    x_tmp->sign = NONNEGATIVE;
    y_tmp->sign = NONNEGATIVE;

    if (strcmp(mulc, "textbook") == 0) {
        bi_mul_textbook(dst, x_tmp, y_tmp);
        bi_delete(&x_tmp);
        bi_delete(&y_tmp);
        return;
    }
    else {
        fprintf(stderr, "mulc name error");
        return;
    }

}
