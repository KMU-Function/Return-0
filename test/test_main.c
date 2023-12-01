#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#if defined(__linux__) || defined(__APPLE__)
#include "../src/api.h"
#elif defined(_WIN32)
#include "api.h"
#endif

#define ITERNUM 100
#define MIN(a, b) a < b ? a : b;
#define MAX(a, b) a > b ? a : b;

void fprint_format(FILE* fp, bigint* x, int idx){
    
    #if DTYPE == 8
        fprintf(fp, "%02x ", x->a[idx]);
    #elif DTYPE == 32
        fprintf(fp, "%08x ", x->a[idx]);
    #elif DTYPE == 64
        fprintf(fp, "%016llx ", x->a[idx]);
    #endif
}

// tmpx = 7ed8c407 73c75ad1 62aafadd 2ddf0cf3 00bfc89b 23d99420 16d8e0f3 288fbec0 337ede46 2570c74a 49abc507 4e5e95f9 5ce559cf 4dd76ec9
// x sign: 0
// x len: 14
// tmpr = 7ed8c407 73c75ad1 62aafadd 2ddf0cf3 00bfc89b 23d99420 16d8e0f2 e1035447 5a3e25af 93d80f5e 4888411a 9daa0646 4682d697 c939d84e

int main(void) {
    srand(time(NULL));

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word* xarr = NULL;
    word* yarr = NULL;

    //! add test*********************************************************
    FILE* fp_add = NULL;
    fp_add = fopen("test/test_add.txt", "w");
    assert(fp_add != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen < ylen ? ylen : xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        bi_add(&z, x, y);


        // char xsign = x->sign == NONNEGATIVE ? '+' : '-';
        // char ysign = y->sign == NONNEGATIVE ? '+' : '-';
        // char zsign = z->sign == NONNEGATIVE ? '+' : '-';

        fprintf(fp_add, "%s", "x: "); 
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_add, x, idx);
        }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "y: ");
        for(int idx = y->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_add, y, idx);
        }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "z: ");
        for(int idx = z->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_add, z, idx);
        }fprintf(fp_add, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        printf("add test [%d] finished\n", iter);
    }
    fclose(fp_add);

    //! sub test*********************************************************
    FILE* fp_sub = NULL;
    fp_sub = fopen("test/test_sub.txt", "w");
    fp_sub = fopen("test/test_sub.txt", "w");
    assert(fp_sub != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        // printf("iter == %d\n\n", iter);

        int xlen = rand() % 100;
        int ylen = rand() % 100;
        // int xlen = 14;
        // int ylen = 14;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen < ylen ? ylen : xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }
        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        // word _xarr[14] = { 0x4dd76ec9, 0x5ce559cf, 0x4e5e95f9, 0x49abc507,0x2570c74a,0x337ede46,0x288fbec0,0x16d8e0f3,0x23d99420,0x00bfc89b,0x2ddf0cf3,0x62aafadd,0x73c75ad1, 0x7ed8c407 };
        // word _yarr[14] = { 0xc939d84e, 0x4682d697, 0x9daa0646 ,0x4888411a, 0x93d80f5e, 0x5a3e25af, 0xe1035447, 0x16d8e0f2, 0x23d99420, 0x00bfc89b, 0x2ddf0cf3, 0x62aafadd, 0x73c75ad1, 0x7ed8c407};

        // bi_set_by_array(&x, NONNEGATIVE, _xarr, 14);
        // bi_set_by_array(&y, NONNEGATIVE, _yarr, 14);

        bi_sub(&z, x, y);

        fprintf(fp_sub, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_sub, x, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "y: ");
        for(int idx = y->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_sub, y, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "z: ");
        for(int idx = z->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_sub, z, idx);
        }fprintf(fp_sub, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        printf("sub test [%d] finished\n", iter);
    }
    fclose(fp_sub);

    //! mul test*********************************************************
    FILE* fp_mul = NULL;
    fp_mul = fopen("test/test_mul.txt", "w");
    assert(fp_mul != NULL);
    
    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        bi_mul(&z, x, y, "textbook");

        fprintf(fp_mul, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_mul, x, idx);
        }fprintf(fp_mul, "%s", "\n");

        fprintf(fp_mul, "%s", "y: ");
        for(int idx = y->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_mul, y, idx);
        }fprintf(fp_mul, "%s", "\n");

        fprintf(fp_mul, "%s", "z: ");
        for(int idx = z->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_mul, z, idx);
        }fprintf(fp_mul, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        printf("mul test [%d] finished\n", iter);
    }
    fclose(fp_mul);

    //! shr bit test*********************************************************
    FILE* fp_shr_bit = NULL;
    fp_shr_bit = fopen("test/test_shr.txt", "w");
    assert(fp_shr_bit != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;
        // int ylen = 1;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);


        fprintf(fp_shr_bit, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_shr_bit, x, idx);
        }fprintf(fp_shr_bit, "%s", "\n");

        int r = rand() % (x->wordlen + 1);
        // int r = (rand() % 12 + 1) * 4;
        bi_shr(&x, r);

        fprintf(fp_shr_bit, "%s", "y: ");
        fprintf(fp_shr_bit, "%d", r);
        fprintf(fp_shr_bit, "%s", "\n");

        fprintf(fp_shr_bit, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_shr_bit, x, idx);
        }fprintf(fp_shr_bit, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        printf("shr test [%d] finished\n", iter);
    }
    fclose(fp_shr_bit);

    //! shl test*********************************************************
    FILE* fp_shl_bit = NULL;
    fp_shl_bit = fopen("test/test_shl.txt", "w");
    assert(fp_shl_bit != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = rand() % 10;
        int ylen = rand() % 10;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);


        fprintf(fp_shl_bit, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_shl_bit, x, idx);
        }fprintf(fp_shl_bit, "%s", "\n");

        int r = rand() % (x->wordlen + 1);
        bi_shl(&x, r);

        fprintf(fp_shl_bit, "%s", "y: ");
        fprintf(fp_shl_bit, "%d", r);
        fprintf(fp_shl_bit, "%s", "\n");

        fprintf(fp_shl_bit, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_shl_bit, x, idx);
        }fprintf(fp_shl_bit, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        printf("shl test [%d] finished\n", iter);
    }
    fclose(fp_shl_bit);

    //! sqr test*********************************************************
    FILE* fp_sqr = NULL;
    fp_sqr = fopen("test/test_sqr.txt", "w");
    assert(fp_sqr != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = rand() % 10;

        bi_new(&x, xlen);
        bi_new(&z, 2 * xlen);

        xarr = (word*)calloc(xlen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);

        bi_sqr(&z, x, "textbook");

        fprintf(fp_sqr, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_sqr, x, idx);
        }fprintf(fp_sqr, "%s", "\n");

        fprintf(fp_sqr, "%s", "z: ");
        for(int idx = z->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_sqr, z, idx);
        }fprintf(fp_sqr, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&z);

        free(xarr);
        printf("sqr test [%d] finished\n", iter);
    }
    fclose(fp_sqr);

    // //! Karatsuba test*********************************************************
    // FILE* fp_krt = NULL;
    // fp_krt = fopen("test/test_krt.txt", "w");
    // assert(fp_krt != NULL);
    // for (int iter = 0; iter < ITERNUM; iter++) {
    //     int xlen = rand() % 2;
    //     int ylen = rand() % 2;

    //     bi_new(&x, xlen);
    //     bi_new(&y, ylen);
    //     bi_new(&z, xlen + ylen);

    //     xarr = (word*)calloc(xlen, sizeof(word));
    //     yarr = (word*)calloc(ylen, sizeof(word));

    //     for(int i = 0; i < xlen; i++){
    //         xarr[i] = rand();
    //     }
    //     for(int i = 0; i < ylen; i++){
    //         yarr[i] = rand();
    //     }

    //     bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
    //     bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

    //     karatsuba_mul(&z, x, y);

    //     fprintf(fp_krt, "%s", "x: ");
    //     for(int idx = x->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_krt, x, idx);
    //     }fprintf(fp_krt, "%s", "\n");

    //     fprintf(fp_krt, "%s", "y: ");
    //     for(int idx = y->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_krt, y, idx);
    //     }fprintf(fp_krt, "%s", "\n");

    //     fprintf(fp_krt, "%s", "z: ");
    //     for(int idx = z->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_krt, z, idx);
    //     }fprintf(fp_krt, "%s", "\n\n");

    //     bi_delete(&x);
    //     bi_delete(&z);

    //     free(xarr);
    // }
    // fclose(fp_krt);

    
    //! div test*********************************************************
    FILE* fp_div = NULL;
    fp_div = fopen("test/test_div.txt", "w");
    assert(fp_div != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int tmp1 = (rand() % 10) + 1;
        int tmp2 = (rand() % 10) + 1;
        if(tmp1 == tmp2){
            tmp1++;
        }

        int xlen = MAX(tmp1, tmp2);
        int ylen = MIN(tmp1, tmp2);

        bigint* q = NULL;
        bigint* r = NULL;

        bi_new(&x, xlen);
        bi_new(&y, ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        if(bi_binary_longdiv(&q, &r, x, y) == -1){
            goto EXIT;
        }

        fprintf(fp_div, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_div, x, idx);
        }fprintf(fp_div, "%s", "\n");

        fprintf(fp_div, "%s", "y: ");
        for(int idx = y->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_div, y, idx);
        }fprintf(fp_div, "%s", "\n");

        fprintf(fp_div, "%s", "q: ");
        for(int idx = q->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_div, q, idx);
        }fprintf(fp_div, "%s", "\n\n");

EXIT:
        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&q);
        bi_delete(&r);

        free(xarr);
        free(yarr);
        printf("div test [%d] finished\n", iter);
    }
    fclose(fp_div);

    // //! barrett test*********************************************************
    // FILE* fp_bar = NULL;
    // fp_bar = fopen("test/test_bar.txt", "w");
    // assert(fp_bar != NULL);
    // for (int iter = 0; iter < ITERNUM; iter++) {

    //     int ylen = (rand() % 10) + 1;
    //     int xlen = ylen * 2;
        

    //     bigint* r = NULL;

    //     bi_new(&x, xlen);
    //     bi_new(&y, ylen);

    //     xarr = (word*)calloc(xlen, sizeof(word));
    //     yarr = (word*)calloc(ylen, sizeof(word));

    //     for(int i = 0; i < xlen; i++){
    //         xarr[i] = rand();
    //     }
    //     for(int i = 0; i < ylen; i++){
    //         yarr[i] = rand();
    //     }

    //     bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
    //     bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

    //     bi_barrett_reduction(&r, x, y);
        
    //     fprintf(fp_bar, "%s", "x: ");
    //     for(int idx = x->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_bar, x, idx);
    //     }fprintf(fp_bar, "%s", "\n");

    //     fprintf(fp_bar, "%s", "y: ");
    //     for(int idx = y->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_bar, y, idx);
    //     }fprintf(fp_bar, "%s", "\n");

    //     fprintf(fp_bar, "%s", "r: ");
    //     for(int idx = r->wordlen - 1; idx >= 0; idx--){
    //         fprint_format(fp_bar, r, idx);
    //     }fprintf(fp_bar, "%s", "\n\n");

    //     bi_delete(&x);
    //     bi_delete(&y);
    //     bi_delete(&r);


    //     free(xarr);
    //     free(yarr);
    //     printf("bar test [%d] finished\n", iter);
    // }
    // fclose(fp_bar);


    //! exp test*********************************************************
    FILE* fp_exp = NULL;
    fp_exp = fopen("test/test_exp.txt", "w");
    assert(fp_exp != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        // printf("exp %d\n", iter);

        int xlen = rand() % 10;
        // int xlen = 1;
        // int ylen = rand() % 100;
        int ylen = 1;

        // bigint* x = NULL;
        // bigint* y = NULL;
        // bigint* z = NULL;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
            // xarr[i] = 0x12345678;
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand() % 10;
            // yarr[i] = 5;
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

    
        bi_LtR(&z, &x, y);

        fprintf(fp_exp, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_exp, x, idx);
        }fprintf(fp_exp, "%s", "\n");

        fprintf(fp_exp, "%s", "y: ");
        for(int idx = y->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_exp, y, idx);
        }fprintf(fp_exp, "%s", "\n");

        fprintf(fp_exp, "%s", "z: ");
        for(int idx = z->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_exp, z, idx);
        }fprintf(fp_exp, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
    printf("exp test [%d] finished\n", iter);
    }
    fclose(fp_exp);


    return 0;
}
