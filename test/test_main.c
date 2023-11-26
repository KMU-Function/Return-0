#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../src/api.h"

#define ITERNUM 100

void fprint_format(FILE* fp, bigint* x, int idx){
    #if DTYPE == 8
        fprintf(fp, "%02x ", x->a[idx]);
    #elif DTYPE == 32
        fprintf(fp, "%08x ", x->a[idx]);
    #elif DTYPE == 64
        fprintf(fp, "%016x ", x->a[idx]);
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
        bi_shr(x, r);

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

    return 0;
}
