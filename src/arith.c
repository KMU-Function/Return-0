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
        (*dst)->sign ^= 1;      // 0 - y = -y
    }
    else if(bi_is_zero(y) || y == NULL){
        bi_assign(dst, x);      // x - 0 = x
    }
    else if ((y->sign == NONNEGATIVE) && (compare(x, y) >= 0)) {
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

    //bigint *c = NULL;
    bigint *t = NULL;

    //bi_new(&c, x->wordlen + y->wordlen);
    bi_new(dst, x->wordlen + y->wordlen);
    bi_new(&t, 2);

    for (int j = 0; j < x->wordlen; j++) {
        for (int i = 0; i < y->wordlen; i++) {
            bi_mul_singleword(t->a, x->a[j], y->a[i]);
            bi_shl(&t, (i + j) * 8 * sizeof(word));
            //bi_assign(&c, *dst);    // 불필요한 복사같은데 어떻게 수정할까?
            bi_addc(dst, *dst, t);
            bi_new(&t, 2);
        }
    }

    //bi_delete(&c);
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

void karatsuba_mul_core(bigint** dest, bigint* src1, bigint* src2, uint64_t len) {
    if (len == 1) {
        bi_mul(dest, src1, src2, "textbook");
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

    bi_sub(&D, C, A);
    bi_sub(&D, D, B);

    bi_shl(&D, len * (8 * sizeof(word)));
    len <<= 1;
    bi_shl(&A, len * (8 * sizeof(word)));

    bi_add(dest, D, B);
    bi_add(dest, *dest, A);

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
    karatsuba_mul_core(dest, src1, src2, len);
    (*dest)->sign = src1->sign ^ src2->sign;
}


int bi_binary_longdiv(bigint** q, bigint** r, bigint* a, bigint* b){
    if(a == NULL || b == NULL){
        return -1;
    }
    if(compare_abs(a, b) == -1){
        return -1;
    }

    bigint* one = NULL;
    bi_set_zero(q);
    bi_set_zero(r);     // q = r = 0
    
    for(int j = (a->wordlen * sizeof(word) * 8); j >= 0; j--){
        int aj = get_ith_bit(a, j);
        bi_add(&r, r, aj);      // r = 2 * r + aj

        if(compare(r, b) >= 0){
            
        }

    }

}

// void binary_DIVISION(bigint** dest_Q, bigint** dest_R, bigint* src1, bigint* src2) {
//     uint64_t n = src1->wordlen * sizeof(word) * 8;

//     bigint* Q = NULL;
//     bigint* R = NULL;
//     bi_set_zero(&Q);
//     bi_set_zero(&R);
//     bigint* one = NULL;
//     for (int j = n-1 ; j >= 0; j--) {
//         printf("%d\n", j);
//         bi_shl(&R, 1);
//         R->a[0] += get_ith_bit(src1, j);  // R = (R << 1) ⊕ aj

//         while (compare_abs(R, src2) >= 0) {
//             bi_set_one(&one);
//             bi_shl(&one, j);
//             // Q = Q + 2^j, R = R - B
//             //printf("q len: %d\n", Q->wordlen);
//             bi_expand(Q, 1 + Q->wordlen);
//             bi_add(&Q, Q, one);

//             bi_expand(R, src2->wordlen);
//             bi_sub(&R, R, src2);
//             printf("while end / for : %d\n", j);

//         }
//     }

//     // 결과 반환
//     bi_set_by_array(dest_Q, NONNEGATIVE, Q->a, Q->wordlen);
//     bi_set_by_array(dest_R, NONNEGATIVE, R->a, R->wordlen);
//     bi_delete(&Q);

//     bi_delete(&R);
// }

// int DIVISION(bigint** dest_Q, bigint** dest_R, bigint* src1, bigint* src2) {
//     int com = compare(src1, src2);
//     //printf("com = %d", com);
//     if (bi_is_zero(src2)) {
//         printf("Cannot divide by 0\n");
//         return -1;
//     }
//     if (com == -1) {//src1<src2
//         //printf("DIVISION_if\n");
//         //몫은 0 나머지는 src1
//         //dest_Q = src2
//         bi_set_zero(dest_Q);
//         //dest_R = src1
//         bi_set_by_array(dest_R, src1->sign, src1->a, src1->wordlen);
//     }
//     else if (com == 1) {//src1>src2
//         //printf("DIVISION_elif\n");
//         binary_DIVISION(dest_Q, dest_R, src1, src2);
//     }
//     else {
//         //printf("DIVISION_else\n");
//         //동일한 경우 몫은 1 나머지 0
//         bi_set_one(dest_Q);
//         bi_set_zero(dest_R);
//     }
//     return 0;
// }



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