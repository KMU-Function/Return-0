/**
 * @file arith.c
 * @brief Source file for big integer arithmetic operations.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"
#include "bigint.h"
#include "arith.h"


 /**
  * @brief Calculate the addition of two words x, y considering carry.
  * @param dst Pointer to the destination where the result of x + y will be stored.
  * @param x First word to be added.
  * @param y Second word to be added.
  * @param c Carry from the previous addition.
  * @return The next carry after the addition.
  */
word bi_addcc(word* dst, word x, word y, word c) {
    word nc;     // next carry

    *dst = x + y;
    nc = *dst < x;

    *dst = *dst + c;
    nc = (*dst < c) + nc;
    return nc;
}

/**
 * @brief Calculate the addition of two BigIntegers x, y with the same sign.
 * @details Assumes x and y are non-zero.
 * @param dst Pointer to the destination where the result of x + y will be stored.
 * @param x First BigInt operand.
 * @param y Second BigInt operand.
 * @return 1 if the addition is successful, -1 otherwise.
 */
int bi_addc(bigint** dst, bigint* x, bigint* y) {
    int c = 0;
    int j, n = x->wordlen, m = y->wordlen;      // n >= m

    bi_new(dst, n + 1);

    bigint* tmp_y = NULL;
    if (n == m) {
        bi_assign(&tmp_y, y);
    }
    else if (n > m) {
        bi_new(&tmp_y, n);
        copy_array(tmp_y->a, y->a, m);
        tmp_y->sign = y->sign;
        for (j = m; j < n; j++) {
            tmp_y->a[j] = 0;
        }
    }

    for (j = 0; j < n; j++) {
        c = bi_addcc(&((*dst)->a[j]), x->a[j], tmp_y->a[j], c);
    }

    (*dst)->a[n] = c;
    bi_refine(*dst);

    bi_delete(&tmp_y);
    return 1;
}

/**
 * @brief Calculate the addition of two BigIntegers x, y.
 * @param dst Pointer to the destination where the result of x + y will be stored.
 * @param x First BigInt operand.
 * @param y Second BigInt operand.
 * @return 1 if the addition is successful, -1 otherwise.
 * @see bi_addc, bi_addcc
 */
int bi_add(bigint** dst, bigint* x, bigint* y) {
    bigint* absx = NULL;
    bigint* absy = NULL;
    bigint* tmp = NULL;

    // x = 0
    if (bi_is_zero(x) == 1 || x == NULL) {
        bi_assign(&tmp, y);
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return 1;
    }
    // y = 0
    else if (bi_is_zero(y) == 1 || y == NULL) {
        bi_assign(&tmp, x);
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return 1;
    }
    // x > 0 and y < 0
    else if ((x->sign == NONNEGATIVE) && (y->sign == NEGATIVE)) {
        bi_assign(&absy, y);
        bi_flip_sign(absy);
        bi_sub(&tmp, x, absy);
        bi_assign(dst, tmp);
        bi_delete(&absy);
        bi_delete(&tmp);
        return 1;
    }
    // x < 0 and y > 0 
    else if ((x->sign == NEGATIVE) && (y->sign == NONNEGATIVE)) {
        bi_assign(&absx, x);
        bi_flip_sign(absx);
        bi_sub(&tmp, y, absx);
        bi_assign(dst, tmp);
        bi_delete(&absx);
        bi_delete(&tmp);
        return 1;
    }
    // x > 0 and y > 0 or 
    // x < 0 and y < 0
    else if (x->wordlen >= y->wordlen) {
        bi_addc(&tmp, x, y);
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return 1;
    }
    else {
        bi_addc(&tmp, y, x);
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return 1;
    }

    return -1;
}

/**
 * @brief Calculate subtraction of two words (x - y) with borrow consideration.
 *        The result is stored at the destination pointed by dst.
 * @param dst Pointer to the destination where the result of x - y will be stored.
 * @param x The first word operand.
 * @param y The second word operand.
 * @param b The borrow from the previous subtraction.
 * @return The next borrow for the subsequent subtraction.
 */
word bi_subcc(word* dst, word x, word y, word b) {
    word nb;     // next borrow

    *dst = x - b;
    nb = (x < b);
    nb = nb + (*dst < y);
    *dst = *dst - y;

    return nb;
}

/**
 * @brief Calculate subtraction of two positive BigInts (x - y) where 0 < y <= x.
 *        The result is stored at the destination pointed by dst.
 * @param dst Pointer to the destination where the result of x - y will be stored.
 * @param x Pointer to the first positive BigInt operand.
 * @param y Pointer to the second positive BigInt operand.
 * @return Always returns 1.
 */
int bi_subc(bigint** dst, bigint* x, bigint* y) {
    int b;
    size_t j, n = x->wordlen, m = y->wordlen;      // n >= m

    bi_new(dst, n);
    bigint* tmp_y = NULL;

    if (n == m) {
        bi_assign(&tmp_y, y);
    }
    else if (n > m) {
        bi_new(&tmp_y, n);
        copy_array(tmp_y->a, y->a, m);
        tmp_y->sign = y->sign;
        for (j = m; j < n; j++) {
            tmp_y->a[j] = 0;
        }
    }

    b = 0;
    for (j = 0; j < n; j++) {
        b = bi_subcc(&((*dst)->a[j]), x->a[j], tmp_y->a[j], b);
    }

    bi_delete(&tmp_y);
    bi_refine(*dst);
    return 1;
}

/**
 * @brief Calculate subtraction of two BigInts (x - y).
 *        The result is stored at the destination pointed by dst.
 * @param dst Pointer to the destination where the result of x - y will be stored.
 * @param x Pointer to the first BigInt operand.
 * @param y Pointer to the second BigInt operand.
 * @return 0 if successful, -1 if an error occurs.
 */
int bi_sub(bigint** dst, bigint* x, bigint* y) {
    int cmp;
    bigint* tmp_x = NULL, * tmp_y = NULL;
    bigint* tmp = NULL;

    // x = 0
    if (bi_is_zero(x) || x == NULL) {
        bi_assign(&tmp, y);
        bi_assign(dst, tmp);
        bi_flip_sign(*dst);
        bi_delete(&tmp);

        return 0;
    }
    // y = 0
    else if (bi_is_zero(y) || y == NULL) {
        bi_assign(&tmp, x);
        bi_assign(dst, tmp);
        bi_delete(&tmp);

        return 0;
    }

    cmp = compare(x, y);

    // x = y
    if (cmp == 0) {
        bi_set_zero(dst);
        return 0;
    }
    // x, y >= 0
    if ((x->sign == NONNEGATIVE) && (y->sign == NONNEGATIVE)) {
        // printf("(x->sign == NONNEGATIVE) && (y->sign == NONNEGATIVE)\n");
        // x >= y >= 0
        if (cmp == 1) {
            // printf("(x->sign == NONNEGATIVE) && (y->sign == NONNEGATIVE) --> (cmp == 1)\n");
            bi_subc(&tmp, x, y);
            bi_assign(dst, tmp);
            bi_delete(&tmp);
        }
        // y > x >= 0 
        else if (cmp == -1) {
            // printf("(x->sign == NONNEGATIVE) && (y->sign == NONNEGATIVE) --> (cmp == -1)\n");
            bi_subc(&tmp, y, x);
            bi_assign(dst, tmp);
            (*dst)->sign = NEGATIVE;
            bi_delete(&tmp);
        }

        return 0;
    }
    // x, y < 0 
    else if ((x->sign == NEGATIVE) && (y->sign == NEGATIVE)) {
        // printf("(x->sign == NEGATIVE) && (y->sign == NEGATIVE)\n");
        bi_assign(&tmp_x, x);
        tmp_x->sign = NONNEGATIVE;
        bi_assign(&tmp_y, y);
        tmp_y->sign = NONNEGATIVE;

        if (cmp == 1) {
            // printf("(x->sign == NEGATIVE) && (y->sign == NEGATIVE) --> (cmp == 1)\n");
            bi_subc(&tmp, tmp_y, tmp_x);
            bi_assign(dst, tmp);
            bi_delete(&tmp);
        }
        else if (cmp == -1) {
            // printf("(x->sign == NEGATIVE) && (y->sign == NEGATIVE) --> (cmp == -1)\n");
            bi_subc(&tmp, tmp_x, tmp_y);
            bi_assign(dst, tmp);
            (*dst)->sign = NEGATIVE;
            bi_delete(&tmp);
        }

        bi_delete(&tmp_x);
        bi_delete(&tmp_y);
        return 0;
    }
    /* x > 0 and y < 0 : add(x, |y|) */
    else if ((x->sign == NONNEGATIVE) && (y->sign == NEGATIVE)) {
        bi_assign(&tmp_y, y);
        tmp_y->sign = NONNEGATIVE;

        bi_add(&tmp, x, tmp_y);
        bi_assign(dst, tmp);
        bi_delete(&tmp);

        bi_delete(&tmp_y);
        return 0;
    }
    /* x < 0 and y > 0 : -add(|x|, y) */
    else {
        bi_assign(&tmp_x, x);
        tmp_x->sign = NONNEGATIVE;

        bi_add(&tmp, tmp_x, y);
        bi_assign(dst, tmp);
        (*dst)->sign = NEGATIVE;

        bi_delete(&tmp);
        bi_delete(&tmp_x);
        return 0;
    }

    return -1;
}

/**
 * @brief Calculate multiplication of two words (x * y) using a specialized algorithm.
 *        The result is stored at the destination pointed by dst.
 * @param dst Pointer to the destination where the result of x * y will be stored.
 * @param x The first word operand.
 * @param y The second word operand.
 */
void bi_mulc(word* dst, word x, word y) {
    word x0;
    word y0;

    if (DTYPE == 8) {
        x0 = x & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
        y0 = y & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
    }
    else if (DTYPE == 32) {
        x0 = x & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
        y0 = y & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
    }
    else if (DTYPE == 64) {
        x0 = x & ((1ULL << (sizeof(word) * 8 / 2)) - 1);
        y0 = y & ((1ULL << (sizeof(word) * 8 / 2)) - 1);
    }
    word x1 = x >> (sizeof(word) * 8 / 2);
    word y1 = y >> (sizeof(word) * 8 / 2);

    word t, t0, t1, c0, c1;

    t0 = x0 * y1;
    t1 = x1 * y0;
    t0 = t0 + t1;
    t1 = t0 < t1;

    c0 = x0 * y0;
    c1 = x1 * y1;
    t = c0;
    c0 = c0 + (t0 << (sizeof(word) * 8 / 2));
    c1 = c1 + (t1 << (sizeof(word) * 8 / 2)) + (t0 >> (sizeof(word) * 8 / 2)) + (c0 < t);

    dst[1] = c1;
    dst[0] = c0;
    return;
}

/**
 * @brief Calculate multiplication of two BigInts (x * y) using the textbook method.
 * @param dst Pointer to the destination where the result of x * y will be stored.
 * @param x Pointer to the first positive BigInt operand.
 * @param y Pointer to the second positive BigInt operand.
 * @return Always returns 0.
 */
int bi_mul_textbook(bigint** dst, bigint* x, bigint* y) {
    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        abort();
    }

    int i, j, n = x->wordlen, m = y->wordlen;
    bigint* c = NULL, * t = NULL;

    bi_new(&c, n + m);
    bi_new(&t, 2);

    for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
            bi_mulc(t->a, x->a[j], y->a[i]);
            bi_shl(&t, (sizeof(word) * 8) * (i + j));     // t <- t << w(i+j);
            bi_add(dst, c, t);
            bi_assign(&c, *dst);
            bi_new(&t, 2);
        }
    }

    bi_refine(*dst);
    bi_delete(&c);
    bi_delete(&t);
    return 0;
}

/**
 * @brief Calculate multiplication of two BigInts (x * y) with specified multiplication mode.
 * @param dst Pointer to the destination where the result of x * y will be stored.
 * @param x Pointer to the first BigInt operand.
 * @param y Pointer to the second BigInt operand.
 * @param mulc String specifying the multiplication mode ("textbook", "improved", "karatsuba").
 */
void bi_mul(bigint** dst, bigint* x, bigint* y, const char* mulc) {
    bigint* z_tmp = NULL;

    // x = 0 or y = 0
    if (bi_is_zero(x) || bi_is_zero(y)) {
        bi_set_zero(dst);
        return;
    }

    // x = 1
    if (bi_is_one(x)) {
        bi_assign(&z_tmp, y);
        bi_assign(dst, z_tmp);
        bi_delete(&z_tmp);
        return;
    }

    // y = 1
    if (bi_is_one(y)) {
        bi_assign(&z_tmp, x);
        bi_assign(dst, z_tmp);
        bi_delete(&z_tmp);
        return;
    }

    bigint* tmp = NULL;
    // x = -1
    bi_assign(&tmp, x);
    bi_flip_sign(tmp);
    if (bi_is_one(tmp)) {
        bi_assign(&z_tmp, y);
        bi_assign(dst, z_tmp);
        bi_flip_sign(*dst);
        bi_delete(&z_tmp);
        bi_delete(&tmp);
        return;
    }

    // y = -1
    bi_assign(&tmp, y);
    bi_flip_sign(tmp);
    if (bi_is_one(tmp)) {
        bi_assign(&z_tmp, x);
        bi_assign(dst, z_tmp);
        bi_flip_sign(*dst);
        bi_delete(&z_tmp);
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

    //! must do
    bi_new(&z_tmp, 1);

    if (strcmp(mulc, "textbook") == 0) {
        bi_mul_textbook(&z_tmp, x_tmp, y_tmp);
        bi_assign(dst, z_tmp);
    }
    else if (strcmp(mulc, "improved") == 0) {
        bi_mul_improvedtextbook(&z_tmp, x_tmp, y_tmp);
        bi_assign(dst, z_tmp);
    }
    if (strcmp(mulc, "karatsuba") == 0) {
        bi_mul_karatsuba(&z_tmp, x_tmp, y_tmp);
        bi_assign(dst, z_tmp);
    }
    else {
        fprintf(stderr, "mulc name error");
        bi_delete(&x_tmp);
        bi_delete(&y_tmp);
        bi_delete(&z_tmp);
        return;
    }
    // bi_show_hex_inorder(*dst);

    // consider sign of result
    (*dst)->sign = x->sign ^ y->sign;
    bi_delete(&x_tmp);
    bi_delete(&y_tmp);
    bi_delete(&z_tmp);

    return;
}

/**
 * @brief Calculate multiplication of two BigInts (x * y) using an improved textbook method.
 * @param dst Pointer to the destination where the result of x * y will be stored.
 * @param x Pointer to the first positive BigInt operand.
 * @param y Pointer to the second positive BigInt operand.
 */
void bi_mul_improvedtextbook(bigint** dst, bigint* x, bigint* y) {
    uint64_t n = x->wordlen;
    uint64_t m = y->wordlen;

    if (n % 2 == 1) {
        x->wordlen = n + 1;
        x->a = (word*)realloc(x->a, sizeof(word) * (n + 1));
        x->a[n] = 0x00;
        n += 1;
    }

    bigint* temp_int1 = NULL;
    bigint* temp_int2 = NULL;
    bigint* temp_result = NULL;
    bi_new(&temp_int1, n + m);
    bi_new(&temp_int2, n + m);
    bi_new(&temp_result, n + m);
    temp_int1->sign = (*dst)->sign;
    temp_int2->sign = (*dst)->sign;
    temp_result->sign = (*dst)->sign;

    bigint* _dst = NULL;

    bi_set_zero(dst);
    for (uint64_t i = 0; i < m; i++) {
        memset(temp_int1->a, 0, sizeof(word) * temp_int1->wordlen);
        memset(temp_int2->a, 0, sizeof(word) * temp_int2->wordlen);
        for (uint64_t j = 0; j < (n / 2); j++) {
            word A1 = x->a[2 * j];
            word B2 = y->a[i];
            word A3 = x->a[2 * j + 1];
            word B4 = y->a[i];
            bi_mulc(temp_int1->a + (2 * j + i), A1, B2);
            bi_mulc(temp_int2->a + (2 * j + 1 + i), A3, B4);
        }
        //T<-ADDC(T0,T1)

        // bi_expand(temp_int1, temp_int2);
        temp_int1->wordlen > temp_int2->wordlen ? bi_expand(temp_int2, temp_int1->wordlen) : bi_expand(temp_int1, temp_int2->wordlen);
        bi_add(&temp_result, temp_int1, temp_int2);

        //T<-T<<i*sizeof(word)*8
        //C<- ADDC(C,T)
        // bi_expand(*dst, temp_result);
        (*dst)->wordlen > temp_result->wordlen ? bi_expand(temp_result, (*dst)->wordlen) : bi_expand((*dst), temp_result->wordlen);
        // bi_add(dst, *dst, temp_result);
        bi_add(&_dst, *dst, temp_result);
        bi_assign(dst, _dst);
    }
    bi_refine(x);
    bi_refine(*dst);

    (*dst)->sign = x->sign ^ y->sign;
    bi_delete(&temp_result);
    bi_delete(&temp_int1);
    bi_delete(&temp_int2);
}

/**
 * @brief Calculate squaring of a word (x^2) using a specialized algorithm.
 *        The result is stored at the destination pointed by dst.
 * @param dst Pointer to the destination where the result of x^2 will be stored.
 * @param x The word operand.
 */
void bi_sqrc(word* dst, word x) {

    word a0 = x & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
    // word a0 = x & ((1 << (sizeof(word) * 8 / 2)) - 1);
    word a1 = x >> (sizeof(word) * 8 / 2);

    word c0 = a0 * a0;
    word c1 = a1 * a1;

    word t[2] = { 0x00 };

    dst[1] = c1;
    dst[0] = c0;

    t[0] = a1 * a0;
    t[1] = t[0] >> ((sizeof(word) * 8 / 2) - 1);        // word/2 + 1 bit 
    t[0] = t[0] << ((sizeof(word) * 8 / 2) + 1);        // word/2 - 1 bit

    dst[0] = dst[0] + t[0];
    dst[1] = dst[1] + t[1] + (dst[0] < t[0] ? 1 : 0);   // consider carry
}

/**
 * @brief Calculate squaring of a BigInt (x^2) using the textbook method.
 * @param dst Pointer to the destination where the result of x^2 will be stored.
 * @param x Pointer to the BigInt operand.
 */
void bi_sqr_textbook(bigint** dst, bigint* x) {
    bigint* c1 = NULL;
    bigint* c2 = NULL;
    bigint* c = NULL;
    bigint* t1 = NULL;
    bigint* t2 = NULL;

    bi_new(&c1, 2 * (x->wordlen));
    bi_new(&c2, 2 * (x->wordlen));
    bi_new(&c, 2 * (x->wordlen));
    bi_new(&t1, 2);
    bi_new(&t2, 2);

    for (int j = 0; j < x->wordlen; j++) {
        bi_sqrc(t1->a, x->a[j]);
        bi_shl(&t1, (2 * j) * 8 * sizeof(word));
        bi_add(&c1, t1, c1);
        for (int i = j + 1; i < x->wordlen; i++) {
            bi_mulc(t2->a, x->a[i], x->a[j]);
            bi_shl(&t2, (i + j) * 8 * sizeof(word));
            bi_add(&c2, c2, t2);
            bi_new(&t2, 2);
        }
        bi_new(&t1, 2);
    }
    bi_shl(&c2, 1);

    bi_add(&c, c1, c2);

    bi_assign(dst, c);

    bi_delete(&c1);
    bi_delete(&c2);
    bi_delete(&c);
    bi_delete(&t1);
    bi_delete(&t2);
}

/**
 * @brief Calculate squaring of a BigInt (x^2) with specified squaring mode.
 * @param dst Pointer to the destination where the result of x^2 will be stored.
 * @param x Pointer to the BigInt operand.
 * @param mulc String specifying the squaring mode ("textbook").
 * @details This function calculates the square of a BigInt based on the specified squaring mode.
 *          Currently, only the "textbook" mode is supported.
 */
void bi_sqr(bigint** dst, bigint* x, const char* mulc) {
    bigint* dst_tmp = NULL;

    if (bi_is_zero(x) || bi_is_one(x)) {    // x = 0 or x = 1
        bi_assign(&dst_tmp, x);
        bi_assign(dst, dst_tmp);
        bi_delete(&dst_tmp);
        return;
    }

    bigint* tmp = NULL;
    bi_assign(&tmp, x);
    bi_flip_sign(tmp);
    if (bi_is_one(tmp)) {       // x = -1;
        bi_assign(&dst_tmp, x);
        bi_assign(dst, dst_tmp);
        bi_flip_sign(*dst);
        bi_delete(&dst_tmp);
        bi_delete(&tmp);
        return;
    }

    bi_delete(&tmp);

    // x_tmp는 const로 없앨 수 있을듯
    bigint* x_tmp = NULL;
    bi_assign(&x_tmp, x);
    x_tmp->sign = NONNEGATIVE;

    if (strcmp(mulc, "textbook") == 0) {
        bi_sqr_textbook(&dst_tmp, x_tmp);
        bi_assign(dst, dst_tmp);
        bi_delete(&dst_tmp);
        bi_delete(&x_tmp);
        return;
    }
    else {
        fprintf(stderr, "mulc name error");
        bi_delete(&x_tmp);
        return;
    }

}

/**
 * @brief Core function for Karatsuba multiplication.
 * @details Recursively performs Karatsuba multiplication for given operands.
 * @param dst Pointer to the destination where the result will be accumulated.
 * @param x Pointer to the first positive BigInt operand.
 * @param y Pointer to the second positive BigInt operand.
 * @param len Length of the operands for the current recursion level.
 */
void bi_mul_karatsuba_core(bigint** dst, bigint* x, bigint* y, uint64_t len) {

    bigint* tmp = NULL;

    if (len == 1) {
        bi_mul(&tmp, x, y, "textbook");
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return;
    }
    len >>= 1;

    bigint* A_h = NULL;
    bigint* A_l = NULL;
    bigint* B_h = NULL;
    bigint* B_l = NULL;

    bigint* A = NULL;
    bigint* B = NULL;
    bigint* C = NULL;
    bigint* D = NULL;
    bigint* A_A = NULL;
    bigint* B_B = NULL;

    bi_new(&A_h, x->wordlen - len);
    bi_new(&A_l, len);
    bi_new(&B_h, y->wordlen - len);
    bi_new(&B_l, len);

    copy_array(A_h->a, x->a + len, x->wordlen - len);
    copy_array(A_l->a, x->a, len);
    copy_array(B_h->a, y->a + len, y->wordlen - len);
    copy_array(B_l->a, y->a, len);

    bi_new(&A_A, A_h->wordlen + 1);
    bi_new(&B_B, B_h->wordlen + 1);
    bi_new(&A, A_h->wordlen + B_h->wordlen);
    bi_new(&B, A_l->wordlen + B_l->wordlen);
    bi_new(&C, A_A->wordlen + B_B->wordlen);
    bi_new(&D, A_A->wordlen + B_B->wordlen);

    uint64_t len_A = ((x->wordlen - len) < (y->wordlen - len)) ? (x->wordlen - len) : (y->wordlen - len);

    bi_mul_karatsuba_core(&A, A_h, B_h, len_A);
    bi_mul_karatsuba_core(&B, A_l, B_l, len);

    bi_add(&A_A, A_h, A_l);
    bi_refine(A_A);
    bi_add(&B_B, B_h, B_l);
    bi_refine(B_B);

    uint64_t len_C = (A_A->wordlen < B_B->wordlen) ? A_A->wordlen : B_B->wordlen;

    bi_mul_karatsuba_core(&C, A_A, B_B, len_C);

    // bi_sub(&D, C, A);
    // bi_sub(&D, D, B);
    bi_sub(&tmp, C, A);
    bi_sub(&D, tmp, B);

    // printf("Befor shift = "); bi_show_hex_inorder(A);
    bi_shl(&D, len * (8 * sizeof(word)));
    len <<= 1;
    bi_shl(&A, len * (8 * sizeof(word)));
    // printf("After shift = "); bi_show_hex_inorder(A);

    // bi_add(dst, D, B);
    // bi_add(dst, *dst, A);
    bi_add(&tmp, D, B);
    bi_add(dst, tmp, A);
    // bi_assign(dst, tmp);

    bi_delete(&tmp);

    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&C);
    bi_delete(&D);
    bi_delete(&A_A);
    bi_delete(&B_B);
    bi_delete(&A_h);
    bi_delete(&A_l);
    bi_delete(&B_h);
    bi_delete(&B_l);
}

/**
 * @brief Calculate multiplication of two BigInts (x * y) using the Karatsuba algorithm.
 * @param dst Pointer to the destination where the result of x * y will be stored.
 * @param x Pointer to the first positive BigInt operand.
 * @param y Pointer to the second positive BigInt operand.
 */
void bi_mul_karatsuba(bigint** dst, bigint* x, bigint* y) {
    uint64_t len = (x->wordlen < y->wordlen) ? x->wordlen : y->wordlen;
    bigint* _dst = NULL;
    bi_mul_karatsuba_core(&_dst, x, y, len);
    bi_assign(dst, _dst);
    (*dst)->sign = x->sign ^ y->sign;
}

/**
 * @brief Perform binary long division to calculate the quotient and remainder of two BigInts.
 * @param q Pointer to the destination where the quotient will be stored.
 * @param r Pointer to the destination where the remainder will be stored.
 * @param a Dividend BigInt.
 * @param b Divisor BigInt.
 * @return 0 on success, -1 if inputs are invalid or divisor is zero.
 * @details This function calculates the quotient and remainder of the division a / b using binary long division.
 *          The result is stored in the specified pointers q (quotient) and r (remainder).
 *          If either of the input BigInts is NULL or if the divisor (b) is zero, the function returns -1.
 */
int bi_binary_longdiv(bigint** q, bigint** r, bigint* a, bigint* b) {

    if (a == NULL || b == NULL) {
        return -1;
    }
    if (bi_is_zero(b)) {
        bi_set_zero(q);
        bi_set_zero(r);
        return -1;
    }
    if (bi_is_zero(a)) {
        bi_set_zero(q);
        bi_set_zero(r);
        return 0;
    }

    bigint* one = NULL;
    bigint* tmp_r = NULL;
    bi_new(q, a->wordlen);
    bi_set_zero(r);     // q = r = 0
    bi_set_zero(&tmp_r);

    for (int j = (a->wordlen * sizeof(word) * 8) - 1; j >= 0; j--) {
        bigint* aj = NULL;
        bi_new(&aj, 1);
        aj->a[0] = get_ith_bit(a, j);
        bi_shl(r, 1);
        bi_assign(&tmp_r, *r);
        bi_add(r, tmp_r, aj);      // r = 2 * r + aj

        if (compare(*r, b) >= 0) {
            bi_set_one(&one);
            bi_shl(&one, j);        // one = 2 ^ j

            bi_add(q, *q, one);     // q = q + 2^j
            //(*q)->a[j / (sizeof(word) * 8)] += (word)(1 << (j % (sizeof(word) * 8)));

            bi_assign(&tmp_r, *r);
            bi_sub(r, tmp_r, b);       // r = r - b
        }
        bi_delete(&aj);
    }
    bi_delete(&one);
    bi_delete(&tmp_r);
    bi_refine(*q);
    return 0;
}

/**
 * @brief Compute the Barrett reduction pre-computed value (t) for a given modulus (y).
 * @param t Pointer to the destination where the Barrett reduction pre-computed value will be stored.
 * @param y Modulus BigInt.
 * @details This function calculates the Barrett reduction pre-computed value (t) for the specified modulus (y).
 *          The result is stored in the specified pointer t.
 */
void bi_barrett_compute_t(bigint** t, bigint* y) {
    bigint* r = NULL;
    size_t n = y->wordlen;
    bigint* w = NULL, * tt = NULL;
    bi_set_min_words(&w, NONNEGATIVE, 2 * n + 1); // w^2n
    bi_binary_longdiv(&tt, &r, w, y);
    bi_assign(t, tt);

    bi_delete(&w);
    bi_delete(&tt);
    bi_delete(&r);
}

/**
 * @brief Perform the core steps of Barrett reduction.
 * @param r Pointer to the destination where the result of the reduction will be stored.
 * @param x Dividend BigInt.
 * @param m Modulus BigInt.
 * @param t Pre-computed value for Barrett reduction.
 * @return 0 on success, -1 if inputs are invalid or x->wordlen > 2 * m->wordlen.
 * @details This function performs the core steps of Barrett reduction, calculating the remainder of x divided by m
 *          using the pre-computed value t. The result is stored in the specified pointer r.
 */
int bi_barrett_reduction_core(bigint** r, bigint* x, bigint* m, bigint* t) {
    if (x == NULL || m == NULL || t == NULL) {
        return -1;
    }

    int n = m->wordlen;

    // x->wordlen > 2 * n then return fail
    if (x->wordlen > (n * 2)) {
        return -1;
    }

    bigint* tmp_r = NULL;
    bigint* qhat = NULL;
    bigint* tmp_x = NULL;

    bi_assign(&tmp_x, x);
    bi_shr(&tmp_x, (sizeof(word) * 8) * (n - 1));    // qhat = A >> w(n-1)
    bi_mul(&qhat, tmp_x, t, "textbook");             // qhat = qhat * t
    bi_shr(&qhat, (sizeof(word) * 8) * (n + 1));     // qhat = qhat >> w(n+1)
    bi_mul(&tmp_r, m, qhat, "textbook");             // R = N * qhat
    bi_assign(&tmp_x, x);
    bi_sub(r, tmp_x, tmp_r);

    while (compare(*r, m) >= 0) {
        bi_sub(&tmp_r, *r, m);
        bi_assign(r, tmp_r);
    }

    bi_delete(&qhat);
    bi_delete(&tmp_r);
    bi_delete(&tmp_x);
    return 0;
}

/**
 * @brief Perform Barrett reduction on a given dividend x with a specified modulus m.
 * @param r Pointer to the destination where the result of the reduction will be stored.
 * @param x Dividend BigInt.
 * @param m Modulus BigInt.
 * @return 0 on success, -1 if inputs are invalid or m is zero.
 * @details This function performs Barrett reduction on the dividend x with the specified modulus m.
 *          The result is stored in the specified pointer r. If m is zero, the function aborts.
 */
int bi_barrett_reduction(bigint** r, bigint* x, bigint* m) {
    bigint* t = NULL;
    if (bi_is_zero(m)) {
        abort();
    }
    if (bi_is_one(m)) {
        bi_set_zero(r);
        return 0;
    }
    bi_barrett_compute_t(&t, m);
    bi_barrett_reduction_core(r, x, m, t);
    bi_delete(&t);
    return 0;
}

/**
 * @brief Calculate exponentiation of BigInt x^n using the Left-to-Right method.
 * @param dst Pointer to the destination where the result of x^n will be stored.
 * @param x Pointer to the base BigInt.
 * @param n Exponent BigInt.
 * @details This function calculates the exponentiation of the base x to the power of n using the Left-to-Right method.
 *          The result is stored in the specified pointer dst.
 */
void bi_LtR(bigint** dst, bigint** x, bigint* n) {

    bigint* _x = NULL;
    bi_assign(&_x, *x);  // _x <- x (temp)

    bigint* t = NULL;
    bi_set_one(&t);

    bigint* _t = NULL;
    bi_set_one(&_t);

    bigint* _tsqr = NULL;
    bi_set_one(&_tsqr);

    uint8_t _n = 0;

    for (int i = n->wordlen - 1; i >= 0; i--) {
        for (int j = (sizeof(word) * 8) - 1; j >= 0; j--) {
            bi_sqr(&_tsqr, t, "textbook");
            bi_assign(&t, _tsqr);  // t <- t^2          
            _n = (n->a[i] >> j) & 0x01;
            if (_n == 1) {      // n_i = 1            
                bi_mul(&_t, t, _x, "textbook");
                bi_assign(&t, _t);     // t <- t * x             
            }
        }
    }
    bi_assign(dst, t);

    bi_delete(&_x);
    bi_delete(&t);
    bi_delete(&_t);
    bi_delete(&_tsqr);
}

/**
 * @brief Calculate exponentiation of BigInt x^n with modulo using the Left-to-Right method.
 * @param dst Pointer to the destination where the result of x^n modulo modulo will be stored.
 * @param x Pointer to the base BigInt.
 * @param n Exponent BigInt.
 * @param modulo Modulo BigInt.
 * @details This function calculates the exponentiation of the base x to the power of n with modulo using the Left-to-Right method.
 *          The result is stored in the specified pointer dst.
 *          If the intermediate results (_tsqr and _t) are greater than modulo, Barrett reduction is applied.
 *          If the word length of the intermediate result is not suitable for Barrett reduction, the function aborts.
 */
void bi_LtR_mod(bigint** dst, bigint** x, bigint* n, bigint* modulo) {

    bigint* _x = NULL;
    bi_assign(&_x, *x);  // _x <- x (temp)

    bigint* t = NULL;
    bi_set_one(&t);

    bigint* _t = NULL;
    bi_set_one(&_t);

    bigint* _tsqr = NULL;
    bi_set_one(&_tsqr);

    uint8_t _n = 0;

    for (int i = n->wordlen - 1; i >= 0; i--) {
        for (int j = (sizeof(word) * 8) - 1; j >= 0; j--) {
            bi_sqr(&_tsqr, t, "textbook");

            if (compare(_tsqr, modulo) == 1) {     // _tsqr > modulo  --> do reduction
                if (_tsqr->wordlen > (modulo->wordlen * 2)) {   // not suitable to barret reduction
                    printf("_tsqr wordlen = %d, modulo wordlen = %d\n", _tsqr->wordlen, modulo->wordlen);
                    printf("wordlen is not suitable for barret reduction\n");
                    abort();
                }
                else {
                    bi_barrett_reduction(&t, _tsqr, modulo);    // t <- t^2 mod modulo
                }
            }
            else {      // _tsqr <= modulo
                bi_assign(&t, _tsqr);  // t <- t^2         
            }
            _n = (n->a[i] >> j) & 0x01;
            if (_n == 1) {      // n_i = 1            
                bi_mul(&_t, t, _x, "textbook");
                if (compare(_t, modulo) == 1) {     // _t > modulo  --> do reduction
                    if (_t->wordlen > (modulo->wordlen * 2)) {  // not suitable to barret reduction
                        printf("_t wordlen = %d, modulo wordlen = %d\n", _t->wordlen, modulo->wordlen);
                        printf("wordlen is not suitable for barret reduction\n");
                        abort();
                    }
                    else {
                        bi_barrett_reduction(&t, _t, modulo);     // t <- (t * x) mod modulo
                    }
                }
                else {      // _t <= modulo
                    bi_assign(&t, _t);     // t <- t * x             
                }
            }
        }
    }
    bi_assign(dst, t);

    bi_delete(&_x);
    bi_delete(&t);
    bi_delete(&_t);
    bi_delete(&_tsqr);
}
