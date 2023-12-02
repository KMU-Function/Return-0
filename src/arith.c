#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"
#include "bigint.h"
#include "arith.h"


/**
* @brief Calculate addition of two word x, y
* Consider carry
* @param dst pointer of destination where the result of x + y will be stored
*/
word bi_addcc(word* dst, word x, word y, word c) {
    word nc;     // next carry

    *dst = x + y;     
    nc  = *dst < x; 

    *dst = *dst + c;          
    nc  = (*dst < c) + nc;   
    return nc;
}

/**
* @brief Calculate addition of same signed two BigInt x, y
* Assume x, y != 0
* @param dst pointer of destination where the result of x + y will be stored
*/
int bi_addc(bigint** dst, bigint* x, bigint* y) {
    int c = 0;
    int j, n = x->wordlen, m = y->wordlen;      // n >= m

    bi_new(dst, n + 1);

    bigint *tmp_y = NULL;    
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
* @brief Calculate addition of two BigInt x, y
* @param dst pointer of destination where the result of x + y will be stored
*/
int bi_add(bigint** dst, bigint* x, bigint* y) {
    bigint *absx = NULL;
    bigint *absy = NULL;
    bigint *tmp = NULL;

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
* @brief Calculate subtraction of two word x, y
* Consider burrow
* @param dst pointer of destination where the result of x + y will be stored
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
* @brief Calculate subtraction of two positive BigInt x, y 
* Assume 0 < y <= x
* @param dst pointer of destination where the result of x - y will be stored
*/
int bi_subc(bigint** dst, bigint* x, bigint* y) {
    int b;
    size_t j, n = x->wordlen, m = y->wordlen;      // n >= m
    
    bi_new(dst, n);
    bigint *tmp_y = NULL;    

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
* @brief Calculate subtraction of two BigInt x, y
* @param dst pointer of destination where the result of x - y will be stored
*/
int bi_sub(bigint** dst, bigint* x, bigint* y) {
    int cmp;
    bigint *tmp_x = NULL, *tmp_y = NULL;
    bigint *tmp = NULL;

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
* @brief Calculate multiplication of two word x, y 
* @param dst pointer of destination where the result of x * y will be stored
*/
void bi_mulc(word* dst, word x, word y) {
    word x0;
    word y0;

    if (DTYPE == 8) {
        x0 = x & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
        y0 = y & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
    }
    else if (DTYPE == 32) {
        x0 = x & ((1U << (sizeof(word) * 8 / 2)) - 1);
        y0 = y & ((1U << (sizeof(word) * 8 / 2)) - 1);
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
* @brief Calculate multiplication of two Bigint x, y
* multiplication using textbook method
* @param dst pointer of destination where the result of x * y will be stored
*/
int bi_mul_textbook(bigint** dst, bigint* x, bigint* y) {
    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        abort();
    }

    int i, j, n = x->wordlen, m = y->wordlen;
    bigint *c = NULL, *t = NULL;

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
* @brief Calculate multiplication of two BigInt x, y
* @param mulc mode of multiplication
* @param dst pointer of destination where the result of x * y will be stored
*/
void bi_mul(bigint** dst, bigint* x, bigint* y, const char *mulc) {
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

    //! 이거 안해주면 터지는데 why??
    bi_new(&z_tmp, 1);
    // z_tmp = NULL;

    if (strcmp(mulc, "textbook") == 0) {
        bi_mul_textbook(&z_tmp, x_tmp, y_tmp);
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
* @brief Calculate squaring of word x
* @param dst pointer of destination where the result of x^2 will be stored
*/
void bi_sqrc(word* dst, word x) {
    
    word a0 = x & (((word)1 << (sizeof(word) * 8 / 2)) - 1);
    // word a0 = x & ((1 << (sizeof(word) * 8 / 2)) - 1);
    word a1 = x >> (sizeof(word) * 8 / 2);

    word c0 = a0 * a0;
    word c1 = a1 * a1;

    word t[2] = {0x00};

    dst[1] = c1;
    dst[0] = c0;

    t[0] = a1 * a0;
    t[1] = t[0] >> ((sizeof(word) * 8 / 2) - 1);        // word/2 + 1 bit 
    t[0] = t[0] << ((sizeof(word) * 8 / 2) + 1);        // word/2 - 1 bit

    dst[0] = dst[0] + t[0];
    dst[1] = dst[1] + t[1] + (dst[0] < t[0] ? 1 : 0);   // consider carry
}

/**
* @brief Calculate squaring of Bigint x
* squaring using textbook method
* @param dst pointer of destination where the result of x^2 will be stored
*/
void bi_sqr_textbook(bigint** dst, bigint* x) {
    bigint *c1 = NULL;
    bigint *c2 = NULL;
    bigint *c = NULL;
    bigint *t1 = NULL;
    bigint *t2 = NULL;

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
* @brief Calculate squaring of BigInt x^2
* @param mulc mode of squaring
* @param dst pointer of destination where the result of x^2 will be stored
*/
void bi_sqr(bigint** dst, bigint* x, const char *mulc) {
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

void karatsuba_mul_core(bigint** dest, bigint* src1, bigint* src2, uint64_t len) {

    bigint* tmp = NULL;

    if (len == 1) {
        bi_mul(&tmp, src1, src2, "textbook");
        bi_assign(dest, tmp);
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

    bi_new(&A_h, src1->wordlen - len);
    bi_new(&A_l, len);
    bi_new(&B_h, src2->wordlen - len);
    bi_new(&B_l, len);

    copy_array(A_h->a, src1->a + len, src1->wordlen - len);
    copy_array(A_l->a, src1->a, len);
    copy_array(B_h->a, src2->a + len, src2->wordlen - len);
    copy_array(B_l->a, src2->a, len);

    bi_new(&A_A, A_h->wordlen + 1);
    bi_new(&B_B, B_h->wordlen + 1);
    bi_new(&A, A_h->wordlen + B_h->wordlen);
    bi_new(&B, A_l->wordlen + B_l->wordlen);
    bi_new(&C, A_A->wordlen + B_B->wordlen);
    bi_new(&D, A_A->wordlen + B_B->wordlen);

    uint64_t len_A = ((src1->wordlen - len) < (src2->wordlen - len)) ? (src1->wordlen - len) : (src2->wordlen - len);

    karatsuba_mul_core(&A, A_h, B_h, len_A);
    karatsuba_mul_core(&B, A_l, B_l, len);

    bi_add(&A_A, A_h, A_l);
    bi_refine(A_A);
    bi_add(&B_B, B_h, B_l);
    bi_refine(B_B);

    uint64_t len_C = (A_A->wordlen < B_B->wordlen) ? A_A->wordlen : B_B->wordlen;

    karatsuba_mul_core(&C, A_A, B_B, len_C);

    // bi_sub(&D, C, A);
    // bi_sub(&D, D, B);
    bi_sub(&tmp, C, A);
    bi_sub(&D, tmp, B);

    // printf("Befor shift = "); bi_show_hex_inorder(A);
    bi_shl(&D, len * (8 * sizeof(word)));
    len <<= 1;
    bi_shl(&A, len * (8 * sizeof(word)));
    // printf("After shift = "); bi_show_hex_inorder(A);

    // bi_add(dest, D, B);
    // bi_add(dest, *dest, A);
    bi_add(&tmp, D, B);
    bi_add(dest, tmp, A);
    // bi_assign(dest, tmp);

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

void karatsuba_mul(bigint** dest, bigint* src1, bigint* src2) {
    uint64_t len = (src1->wordlen < src2->wordlen) ? src1->wordlen : src2->wordlen;
    bigint* _dst = NULL;
    karatsuba_mul_core(&_dst, src1, src2, len);
    bi_assign(dest, _dst);
    (*dest)->sign = src1->sign ^ src2->sign;
}

/**
* @brief Calculate division of BigInt x
* @param q number of quotient
* @param r number of remainder
* @param a ?
*/
int bi_binary_longdiv(bigint** q, bigint** r, bigint* a, bigint* b){

    if(a == NULL || b == NULL){
        return -1;
    }
    if(bi_is_zero(b)){
        bi_set_zero(q);
        bi_set_zero(r);
        return -1;
    }
    if(bi_is_zero(a)){
        bi_set_zero(q);
        bi_set_zero(r);
        return 0;
    }

    bigint* one = NULL;
    bigint* tmp_r = NULL;
    bi_new(q, a->wordlen);
    bi_set_zero(r);     // q = r = 0
    bi_set_zero(&tmp_r);     
    
    for(int j = (a->wordlen * sizeof(word) * 8) - 1; j >= 0; j--){
        bigint* aj = NULL;
        bi_new(&aj, 1);
        aj->a[0] = get_ith_bit(a, j);
        bi_shl(r, 1);
        bi_assign(&tmp_r, *r);
        bi_add(r, tmp_r, aj);      // r = 2 * r + aj

        if(compare(*r, b) >= 0){
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


void bi_barrett_compute_t(bigint** t, bigint* y){
    bigint* r = NULL;
    size_t n = y->wordlen;
    bigint *w = NULL, *tt = NULL;
    bi_set_min_words(&w, NONNEGATIVE, 2 * n + 1); // w^2n
    bi_binary_longdiv(&tt, &r, w, y);
    bi_assign(t, tt);

    bi_delete(&w);
    bi_delete(&tt);
    bi_delete(&r);
}

// r <- x mod m
// t is pre-computed value
int bi_barrett_reduction_core(bigint** r, bigint* x, bigint* m, bigint* t){
    if (x == NULL || m == NULL || t == NULL) {
        return -1;
    }

    int n = m->wordlen;

    // x->wordlen > 2 * n then return fail
    if(x->wordlen > (n * 2)){
        return -1;
    }

    bigint* tmp_r = NULL;
    bigint* qhat  = NULL;
    bigint* tmp_x = NULL;

    bi_assign(&tmp_x, x);
    bi_shr(&tmp_x, (sizeof(word) * 8) * (n - 1));    // qhat = A >> w(n-1)
    bi_mul(&qhat, tmp_x, t, "textbook");             // qhat = qhat * t
    bi_shr(&qhat, (sizeof(word) * 8) * (n + 1));     // qhat = qhat >> w(n+1)
    bi_mul(&tmp_r, m, qhat, "textbook");             // R = N * qhat
    bi_assign(&tmp_x, x);
    bi_sub(r, tmp_x, tmp_r);

    while (compare(*r, m) >= 0){
        bi_sub(&tmp_r, *r, m);
        bi_assign(r, tmp_r);
    }

    bi_delete(&qhat);
    bi_delete(&tmp_r);
    bi_delete(&tmp_x);
    return 0;
}

int bi_barrett_reduction(bigint** r, bigint* x, bigint* m){
    bigint* t = NULL;
    if(bi_is_zero(m)){
        abort();
    }
    if(bi_is_one(m)){
        bi_set_zero(r);
        return 0;
    }
    bi_barrett_compute_t(&t, m);
    bi_barrett_reduction_core(r, x, m, t);
    bi_delete(&t);
    return 0;
}

/**
* @brief Calculate exponentiation of x^n
* @param x number of base
* @param n number of power
* @param dst pointer of destination where the result of x^n will be stored
*/
void bi_LtR(bigint** z, bigint** x, bigint* n) {

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
    bi_assign(z, t);
    
    bi_delete(&_x);
    bi_delete(&t);
    bi_delete(&_t);
    bi_delete(&_tsqr);
}

void bi_LtR_mod(bigint** z, bigint** x, bigint* n, bigint* modulo) {

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
                        // printf("_t = "); bi_show_hex_inorder(_t);
                        // printf("modulo = "); bi_show_hex_inorder(modulo);
                        bi_barrett_reduction(&t, _t, modulo);     // t <- (t * x) mod modulo
                        // printf("after reduction _t = "); bi_show_hex_inorder(_t);
                        // printf("after reduction t = "); bi_show_hex_inorder(t);
                    }
                }
                else {      // _t <= modulo
                    bi_assign(&t, _t);     // t <- t * x             
                }
            }
        }
    }
    bi_assign(z, t);
    
    bi_delete(&_x);
    bi_delete(&t);
    bi_delete(&_t);
    bi_delete(&_tsqr);
}
