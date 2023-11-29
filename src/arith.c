#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
    bigint* tmp = NULL;
    if(*dst != NULL){
        bi_assign(&tmp, *dst);
    }else{
        bi_new(&tmp, 1);
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
    if(tmp->wordlen <= x->wordlen){
        bi_expand(tmp, x->wordlen + 1);
    }

    assert(x->wordlen == y->wordlen);
    for(int i = 0; i < x->wordlen; i++){
        tmp->a[i] = x->a[i] + y->a[i] + carry;
        // check carry
        if (tmp->a[i] == x->a[i]) {
            continue;
        }
        if (tmp->a[i] < x->a[i]) {
            carry = 1;
        }
        else {
            carry = 0;
        }
    }

    if(carry == 1 || (*dst)->wordlen == x->wordlen + 1){
        tmp->a[tmp->wordlen - 1] = 1;
    }

    // remove zero words
    bi_refine(tmp);
    bi_assign(dst, tmp);
    bi_delete(&tmp);
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

    if(x->sign != NONNEGATIVE && x->sign != NEGATIVE){
        fprintf(stderr, "x sign error\n");
        printf("%d\n", x->sign);
        abort();
    }
    if(y->sign != NONNEGATIVE && y->sign != NEGATIVE){
        fprintf(stderr, "y sign error\n");
        abort();
    }

    if(bi_is_zero(x) || x == NULL){
        bi_assign(dst, y);
    }
    else if(bi_is_zero(y) || y == NULL){
        bi_assign(dst, x);
    }
    else if ((x->sign == NONNEGATIVE) && (y->sign == NEGATIVE)) {
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
    else if(x->sign == y->sign){
        if(x->wordlen >= y->wordlen){
            bi_addc(dst, x, y);
        }
        else{
            bi_addc(dst, y, x);
        }
    }
    else{
        fprintf(stderr, "sign is not same\n");
        abort();
    }

    //bi_assign(dst, tmp);
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

    if(bi_is_one(x)){
        bi_assign(dst, y);
        return;
    }
    else if(bi_is_one(y)){
        bi_assign(dst, x);
        return;
    }
    else if(bi_is_zero(x) || bi_is_zero(y)){
        bi_set_zero(dst);
        return;
    }

    bigint *c = NULL;
    bigint *t = NULL;

    bi_new(&c, x->wordlen + y->wordlen);
    bi_new(dst, x->wordlen + y->wordlen);
    bi_new(&t, 2);

    for (int j = 0; j < x->wordlen; j++) {
        for (int i = 0; i < y->wordlen; i++) {
            bi_mul_singleword(t->a, x->a[j], y->a[i]);
            bi_shl(&t, (i + j) * 8 * sizeof(word));
            bi_assign(&c, *dst);    // 불필요한 복사같은데 어떻게 수정할까?
            bi_addc(dst, c, t);
            bi_new(&t, 2);
        }
    }

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
        printf("x is one\n");
        bi_assign(dst, y);
        return;
    }

    if (bi_is_one(y)) {
        printf("y is one\n");
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

    //todo 1129 test
    bigint* z_tmp = NULL;
    bi_new(&z_tmp, 1);

    if (strcmp(mulc, "textbook") == 0) {
        bi_mul_textbook(&z_tmp, x_tmp, y_tmp);
        bi_assign(dst, z_tmp);  //todo 1129 test
        bi_delete(&x_tmp);
        bi_delete(&y_tmp);
        bi_delete(&z_tmp);  //todo 1129 test
        return;
    }
    else {
        fprintf(stderr, "mulc name error");
        bi_delete(&x_tmp);
        bi_delete(&y_tmp);
        bi_delete(&z_tmp);  //todo 1129 test
        return;
    }

}

// 부호에 관계 없음
void bi_sqr_singleword(word* dst, word x) {

    word a0 = x & ((1 << (sizeof(word) * 8 / 2)) - 1);
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

// 부호 관계 없음
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
        bi_sqr_singleword(t1->a, x->a[j]);
        bi_shl(&t1, (2 * j) * 8 * sizeof(word));
        bi_add(&c1, t1, c1);
        for (int i = j + 1; i < x->wordlen; i++) {
            bi_mul_singleword(t2->a, x->a[i], x->a[j]);
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

void bi_sqr(bigint** dst, bigint* x, const char *mulc) {
    if (bi_is_zero(x) || bi_is_one(x)) {    // x = 0 or x = 1
        bi_assign(dst, x);
        return;
    }

    bigint* tmp = NULL;
    bi_assign(&tmp, x);
    bi_flip_sign(tmp);
    if (bi_is_one(tmp)) {       // x = -1;
        bi_assign(dst, x);
        bi_flip_sign(*dst);
        bi_delete(&tmp);
        return;
    }

    bi_delete(&tmp);

    bigint* x_tmp = NULL;
    bi_assign(&x_tmp, x);
    x_tmp->sign = NONNEGATIVE;
    
    if (strcmp(mulc, "textbook") == 0) {
        bi_sqr_textbook(dst, x_tmp);
        bi_delete(&x_tmp);
        return;
    }
    else {
        fprintf(stderr, "mulc name error");
        return;
    }

}

int bi_2word_div(word* q, bigint* a, word* b){
    word r = a->a[1];       // r <- a[1]
    (*q) = 0;               // q <- 0
    
    for(int j = (sizeof(word) * 8) - 1; j >= 0; j--){
        if(r >= (1 << (sizeof(word) * 8 - 1))){     // r >= 2^(w-1)
            (*q) += (1 << j);
            r = (2 * r) + ((a->a[0] >> j) & 0x01) - (*b);
        }
        else{
            r = (2 * r) + ((a->a[0] >> j) & 0x01);
            if(r >= (*b)){
                (*q) += (1 << j);
                r -= (*b);
            }
        }
    }
    return 0;
}

int bi_divcc(bigint** q, bigint** r, bigint* a, bigint* b){
    if(a == NULL || b == NULL || a->wordlen == 0 || bi_is_zero(b)){
        return;
    }
    bigint* one = NULL;
    bi_set_one(&one);

    int n = a->wordlen;
    int m = b->wordlen;
    
    bi_new(q, 1);      // q in [0, w)

    if(n == m){
        (*q)->a[0] = a->a[m - 1] / b->a[m - 1];
    }
    if(n == m + 1){
        if(a->a[m] == b->a[m - 1]){
            (*q)->a[0] = MAXWORD;       // q <- w - 1
        }
        else{
            bigint* tmp_a = NULL;
            bi_new(&tmp_a, 2);
            tmp_a->a[0] = a->a[m - 1];
            tmp_a->a[1] = a->a[m];      // tmp_a <- a_m*W + a_(m-1)

            if(bi_2word_div((*q)->a, tmp_a, &(b->a[m - 1])) == -1){
                fprintf(stderr, "2 word div error\n");
                exit(0);
            }
            bi_delete(&tmp_a);
        }
    }

    bigint* tmp = NULL;
    bi_new(&tmp, b->wordlen + (*q)->wordlen + 1);
    printf("b: ");
    for(int i = 0; i < b->wordlen; i++){
        printf("%08x ", b->a[i]);
    }printf("\n");



    bi_mul(&tmp, b, *q, "textbook");    // r <- bq
    bi_sub(r, a, tmp);                  // r <- a - bq
    bi_new(&tmp, 1);
    while ((*r)->sign == NEGATIVE){
        bi_sub(tmp, *q, one);     // q <- q - 1
        bi_add(r, *r, b);       // r <- r + b
        bi_assign(q, tmp);
    }
    printf("q: ");
    for(int i = 0; i < (*q)->wordlen; i++){
        printf("%08x ", (*q)->a[i]);
    }printf("\n");
    bi_delete(&tmp);
    bi_delete(&one);
    if(*q == NULL || q == NULL){
        abort();
    }
}


// /*
//     DIVCC
//      input : Xn-1·W^{n-1} + Xn-2·W^{n-2} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
//      output: Q, R
// */
// int _bi_divcc(bigint **q, bigint **r, bigint *x, bigint *y) {
//     if (x == NULL || y == NULL) {       // Check input
//         return -1;
//     }
    
//     size_t  n = x->wordlen, m = y->wordlen;
//     bigint *a = NULL, *t = NULL;

//     bi_new(q, 1);

//     if (n == m) {
//         (*q)->a[0] = x->a[m-1] / y->a[m-1];     // Q = Xm-1 / Ym-1
//     }
//     else if (n == (m + 1)) {
//         if (x->a[m] != y->a[m-1]) {
//             /*
//                 Q = A / Ym-1
//                 A = Xm·W + Xm-1
//             */
//             bi_new(&a, 2);
//             a->a[1] = x->a[m];
//             a->a[0] = x->a[m-1];

//             if (bi_2word_div(&((*q)->a[0]), a, &(y->a[m-1])) == -1) {
//                 bi_delete(&a);
//                 return -1;
//             }
//         }
//         else {
//             (*q)->a[0] = MAXWORD;
//         }
//     }
//     else {
//         return -1;
//     }

//     /* R = X - YQ */
//     bi_mul(&t, y, *q, "textbook");
//     bi_sub(r, x, t);

//     while ((*r)->sign == NEGATIVE) {        // R < 0
//         /* 
//             Q = Q - 1
//             T = R + Y
//             R = T
//         */
//         (*q)->a[0]--;
//         bi_add(&t, *r, y);
//         bi_assign(r, t);
//     }

//     bi_delete(&a);
//     bi_delete(&t);
//     return 0;
// }

/*
    DIVC
     input : Xm·W^m + Xm-1·W^{m-1} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
     output: Q, R

     if wordlen(X) is in {wordlen(Y), wordlen(Y) + 1}
      else return Q = 0, R = X
*/
int bi_divc(bigint **q, bigint **r, bigint *x, bigint *y) {
    if (x == NULL || y == NULL) {       // Check input
        return -1;
    }

    /*
        X < Y, 
         Q = 0
         R = X
    */
    if (compare_abs(x, y) == -1) {
        bi_set_zero(q);
        bi_assign(r, x);
        bi_refine(*r);
        return 0;
    }

    word    w = 1 << ((sizeof(word) * 8) - 1);
    size_t  m = y->wordlen;
    bigint *a = NULL, *b  = NULL;

    /* 
        Compute k (k >= 0) such that 2^{WORD-1} <= (2^k · Ym-1) < 2^{WORD}
    */
    size_t k = 0;
    while (k < (sizeof(word) * 8)) {
        if ((y->a[m-1] << k) >= w) {
            if ((y->a[m-1] << k) <= MAXWORD) {
                break;
            }
            else {
                return -1;
            }
        }
        k++;
    }

    bi_assign(&a, x);
    bi_shl(&a, k);       // A = 2^k · X 
    bi_assign(&b, y);
    bi_shl(&b, k);       // A = 2^k · Y
    
    if (bi_divcc(q, r, a, b) == -1) {
        bi_delete(&a);
        bi_delete(&b);
        return -1;
    }

    bi_shr(r, k);       // 2^{-k} · R
    bi_refine(*r);

    bi_delete(&a);
    bi_delete(&b);
    return 0;
}

/*
    Multi-Precision Long Division
     input : Xn-1·W^{n-1} + Xn-2·W^{n-2} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
     output: Q, R such that X = YQ + R
*/
int bi_div(bigint **q, bigint **r, bigint *x, bigint *y) {
    if (x == NULL || y == NULL) {       // Check input
        return -1;
    }

    /* 
        X < Y, 
         Q = 0
         R = X
    */
    if (compare_abs(x, y) == -1) {
        bi_set_zero(q);
        bi_assign(r, x);
        return 0;
    }

    size_t i, n = x->wordlen;
    bigint *_q = NULL, *_r1 = NULL, *_r2 = NULL;

    bi_new(q, x->wordlen);
    bi_set_zero(&_r1);

    for (i = n; i > 0; i--) {
        /* R1 = R0·W + Xi */
        bi_shl(&_r1, (sizeof(word) * 8));
        _r1->a[0] = x->a[i-1];
        bi_refine(_r1);

        bi_divc(&_q, &_r2, _r1, y);
        
        bi_assign(&_r1, _r2);       // Update R0
        (*q)->a[i-1] = _q->a[0];    // Q = Qn-1·W^{n-1} + Qn-2·W^{n-2} + ··· + Q0
    }

    bi_assign(r, _r1);
    bi_refine(*q);

    bi_delete(&_q);
    bi_delete(&_r1);
    bi_delete(&_r2);
    return 0;
}



// // compute t
// // x in needed to get N
// void bi_compute_barrett_t(bigint** t, bigint* m, bigint* x){
//     bigint* w = NULL;
//     bigint* tt = NULL;
//     bigint* r = NULL;

//     bi_new(&w, 2 * x->wordlen + 1);
//     w->a[w->wordlen - 1] = 1;
//     bi_div(tt, &r, w, x);
// }

// // r <- x mod m
// // t is pre-computed value
// void bi_barrett_reduction(bigint** r, bigint* x, bigint* m, bigint* t){
//     if (x == NULL || m == NULL || t == NULL) {
//         return -1;
//     }

//     int n = m->wordlen;

//     // x->wordlen > 2 * n then return fail
//     if(x->wordlen > (n << 1)){
//         return -1;
//     }

//     bigint* tmp_r = NULL;
//     bigint* qhat  = NULL;
//     bigint* tmp_x = NULL;

//     bi_assign(&tmp_x, x);
//     bi_shr(tmp_x, (sizeof(word) * 8) * (n - 1));    // qhat = A >> w(n-1)
//     bi_mul(&qhat, tmp_x, t, "Karatsuba");           // qhat = qhat * t
//     bi_shr(qhat, (sizeof(word) * 8) * (n + 1));     // qhat = qhat >> w(n+1)
//     bi_mul(&tmp_r, m, qhat, "Karatsuba");           // R = N * qhat
//     bi_sub(r, m, tmp_r);                            // R = A - R

//     while (compare(r, m) >= 0){
//         bi_sub(&tmp_r, *r, m);
//         bi_assign(r, tmp_r);
//     }

//     bi_delete(&qhat);
//     bi_delete(&tmp_r);
//     bi_delete(&tmp_x);
//     return 0;
// }