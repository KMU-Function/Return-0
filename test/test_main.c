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
        fprintf(fp, "%016lx ", x->a[idx]);
    #endif
}

int main(void){
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


        char xsign = x->sign == NONNEGATIVE ? '+' : '-';
        char ysign = y->sign == NONNEGATIVE ? '+' : '-';
        char zsign = z->sign == NONNEGATIVE ? '+' : '-';

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
    }
    fclose(fp_add);

    //! sub test*********************************************************
    FILE* fp_sub = NULL;
    // fp_sub = fopen("test/test_sub.txt", "w");
    fp_sub = fopen("test/test_sub.txt", "w");
    assert(fp_sub != NULL);
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
    }
    fclose(fp_mul);

    //! shr bit test*********************************************************
    FILE* fp_shr_bit = NULL;
    fp_shr_bit = fopen("test/test_shr.txt", "w");
    assert(fp_shr_bit != NULL);
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


        fprintf(fp_shr_bit, "%s", "x: ");
        for(int idx = x->wordlen - 1; idx >= 0; idx--){
            fprint_format(fp_shr_bit, x, idx);
        }fprintf(fp_shr_bit, "%s", "\n");

        int r = rand() % (x->wordlen + 1);
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
    }
    fclose(fp_sqr);

//     // //! Karatsuba test*********************************************************
//     // FILE* fp_krt = NULL;
//     // fp_krt = fopen("test/test_krt.txt", "w");
//     // assert(fp_krt != NULL);
//     // for (int iter = 0; iter < ITERNUM; iter++) {
//     //     int xlen = rand() % 10;
//     //     int ylen = rand() % 10;

//     //     bi_new(&x, xlen);
//     //     bi_new(&y, ylen);
//     //     bi_new(&z, xlen + ylen);

//     //     xarr = (word*)calloc(xlen, sizeof(word));
//     //     yarr = (word*)calloc(ylen, sizeof(word));

//     //     for(int i = 0; i < xlen; i++){
//     //         xarr[i] = rand();
//     //     }
//     //     for(int i = 0; i < ylen; i++){
//     //         yarr[i] = rand();
//     //     }

//     //     bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
//     //     bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

//     //     karatsuba_mul(&z, x, y);

//     //     fprintf(fp_krt, "%s", "x: ");
//     //     for(int idx = x->wordlen - 1; idx >= 0; idx--){
//     //         fprint_format(fp_krt, x, idx);
//     //     }fprintf(fp_krt, "%s", "\n");

//     //     fprintf(fp_krt, "%s", "y: ");
//     //     for(int idx = y->wordlen - 1; idx >= 0; idx--){
//     //         fprint_format(fp_krt, y, idx);
//     //     }fprintf(fp_krt, "%s", "\n");

//     //     fprintf(fp_krt, "%s", "z: ");
//     //     for(int idx = z->wordlen - 1; idx >= 0; idx--){
//     //         fprint_format(fp_krt, z, idx);
//     //     }fprintf(fp_krt, "%s", "\n\n");

//     //     bi_delete(&x);
//     //     bi_delete(&z);

//     //     free(xarr);
//     // }
//     // fclose(fp_krt);

    
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
    }
    fclose(fp_div);

    // //! barrett test*********************************************************
    // FILE* fp_bar = NULL;
    // fp_bar = fopen("test/test_bar.txt", "w");
    // assert(fp_bar != NULL);
    // for (int iter = 0; iter < 1; iter++) {

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

    //     size_t n = y->wordlen;
    //     bigint *w = NULL, *tt = NULL;
    //     bi_set_min_words(&w, NONNEGATIVE, 2 * n + 1); // w^2n
    //     bi_binary_longdiv(&tt, &r, w, y);

    //     printf("w2= "); bi_show_hex_inorder(w);
    //     printf("x= "); bi_show_hex_inorder(x);
    //     printf("y= "); bi_show_hex_inorder(y);
    //     printf("t= "); bi_show_hex_inorder(tt);
    //     bi_barrett_reduction(&r, x, y, tt);
        
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
    //     bi_delete(&w);
    //     bi_delete(&tt);

    //     free(xarr);
    //     free(yarr);
    // }
    // fclose(fp_bar);


    return 0;
}
