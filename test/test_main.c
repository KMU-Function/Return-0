#ifdef MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#if defined(__linux__) || defined(__APPLE__)
#include "../src/api.h"
#elif defined(_WIN32)
#include "api.h"
#endif

#define ITERNUM 1
#define WORDLEN 10
#define MIN(a, b) a < b ? a : b;
#define MAX(a, b) a > b ? a : b;

void fprint_format(FILE* fp, bigint* x, int idx) {

#if DTYPE == 8
    fprintf(fp, "%02x ", x->a[idx]);
#elif DTYPE == 32
    fprintf(fp, "%08x ", x->a[idx]);
#elif DTYPE == 64
    fprintf(fp, "%016llx ", x->a[idx]);
#endif
}



int main(void) {
    srand(time(NULL));

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word* xarr = NULL;
    word* yarr = NULL;

    uint64_t start;
    uint64_t end;
    uint64_t cc = 0;

    printf("\n\n=================Performance=================\n\n");
    //! add test*********************************************************
    FILE* fp_add = NULL;
    fp_add = fopen("test/test_add.txt", "w");
    assert(fp_add != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;
        xlen = ylen = WORDLEN;
        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen < ylen ? ylen : xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_add(&z, x, y);
        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_add, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_add, x, idx);
        }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_add, y, idx);
        }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_add, z, idx);
        }fprintf(fp_add, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("add test [%d] finished\n", iter);
    }
    fclose(fp_add);
    printf("Addition Cycles: %lu\n", cc);
    cc = 0;

    //! sub test*********************************************************
    FILE* fp_sub = NULL;
    fp_sub = fopen("test/test_sub.txt", "w");
    assert(fp_sub != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = rand() % 100;
        int ylen = rand() % 100;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen < ylen ? ylen : xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }
        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_sub(&z, x, y);
        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_sub, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_sub, x, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_sub, y, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_sub, z, idx);
        }fprintf(fp_sub, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("sub test [%d] finished\n", iter);
    }
    fclose(fp_sub);
    printf("Subtraction Cycles: %lu\n", cc);
    cc = 0;

    //! mul test*********************************************************
    FILE* fp_mul = NULL;
    fp_mul = fopen("test/test_mul.txt", "w");
    assert(fp_mul != NULL);

    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();

        bi_mul_textbook(&z, x, y);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_mul, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_mul, x, idx);
        }fprintf(fp_mul, "%s", "\n");

        fprintf(fp_mul, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_mul, y, idx);
        }fprintf(fp_mul, "%s", "\n");

        fprintf(fp_mul, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_mul, z, idx);
        }fprintf(fp_mul, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        // printf("mul test [%d] finished\n", iter);
    }
    fclose(fp_mul);
    printf("Multiplication Cycles: %lu\n", cc);
    cc = 0;


    //! shr bit test*********************************************************
    FILE* fp_shr_bit = NULL;
    fp_shr_bit = fopen("test/test_shr.txt", "w");
    assert(fp_shr_bit != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = rand() % 100;
        int ylen = rand() % 100;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);


        fprintf(fp_shr_bit, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_shr_bit, x, idx);
        }fprintf(fp_shr_bit, "%s", "\n");

        int r = rand() % (x->wordlen + 1);

        start = cpucycles();
        bi_shr(&x, r);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_shr_bit, "%s", "y: ");
        fprintf(fp_shr_bit, "%d", r);
        fprintf(fp_shr_bit, "%s", "\n");

        fprintf(fp_shr_bit, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_shr_bit, x, idx);
        }fprintf(fp_shr_bit, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("shr test [%d] finished\n", iter);
    }
    fclose(fp_shr_bit);
    printf("Shift Right Cycles: %lu\n", cc);
    cc = 0;

    //! shl test*********************************************************
    FILE* fp_shl_bit = NULL;
    fp_shl_bit = fopen("test/test_shl.txt", "w");
    assert(fp_shl_bit != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = rand() % 10;
        int ylen = rand() % 10;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);


        fprintf(fp_shl_bit, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_shl_bit, x, idx);
        }fprintf(fp_shl_bit, "%s", "\n");

        int r = rand() % (x->wordlen + 1);

        start = cpucycles();

        bi_shl(&x, r);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_shl_bit, "%s", "y: ");
        fprintf(fp_shl_bit, "%d", r);
        fprintf(fp_shl_bit, "%s", "\n");

        fprintf(fp_shl_bit, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_shl_bit, x, idx);
        }fprintf(fp_shl_bit, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("shl test [%d] finished\n", iter);
    }
    fclose(fp_shl_bit);
    printf("Shift Left Cycles: %lu\n", cc);
    cc = 0;

    //! sqr test*********************************************************
    FILE* fp_sqr = NULL;
    fp_sqr = fopen("test/test_sqr.txt", "w");
    assert(fp_sqr != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = rand() % 10;
        xlen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&z, 2 * xlen);

        xarr = (word*)calloc(xlen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);

        start = cpucycles();
        bi_sqr_textbook(&z, x);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_sqr, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_sqr, x, idx);
        }fprintf(fp_sqr, "%s", "\n");

        fprintf(fp_sqr, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_sqr, z, idx);
        }fprintf(fp_sqr, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&z);

        free(xarr);
        //printf("sqr test [%d] finished\n", iter);
    }
    fclose(fp_sqr);
    printf("Squaring Cycles: %lu\n", cc);
    cc = 0;

    //! Karatsuba test*********************************************************
    FILE* fp_krt = NULL;
    fp_krt = fopen("test/test_kst.txt", "w");
    assert(fp_krt != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = (rand() % 100) + 1;
        int ylen = (rand() % 100) + 1;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_mul_karatsuba(&z, x, y);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_krt, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_krt, x, idx);
        }fprintf(fp_krt, "%s", "\n");

        fprintf(fp_krt, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_krt, y, idx);
        }fprintf(fp_krt, "%s", "\n");

        fprintf(fp_krt, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_krt, z, idx);
        }fprintf(fp_krt, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&z);

        free(xarr);
        free(yarr);
    }
    fclose(fp_krt);
    printf("Karatsuba Cycles: %lu\n", cc);
    cc = 0;

    //! div test*********************************************************
    FILE* fp_div = NULL;
    fp_div = fopen("test/test_div.txt", "w");
    assert(fp_div != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int tmp1 = (rand() % 10) + 1;
        int tmp2 = (rand() % 10) + 1;
        if (tmp1 == tmp2) {
            tmp1++;
        }

        int xlen = MAX(tmp1, tmp2);
        int ylen = MIN(tmp1, tmp2);
        xlen = ylen = WORDLEN;

        bigint* q = NULL;
        bigint* r = NULL;

        bi_new(&x, xlen);
        bi_new(&y, ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        if (bi_binary_longdiv(&q, &r, x, y) == -1) {
            goto EXIT;
        }

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_div, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_div, x, idx);
        }fprintf(fp_div, "%s", "\n");

        fprintf(fp_div, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_div, y, idx);
        }fprintf(fp_div, "%s", "\n");

        fprintf(fp_div, "%s", "q: ");
        for (int idx = q->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_div, q, idx);
        }fprintf(fp_div, "%s", "\n\n");

    EXIT:
        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&q);
        bi_delete(&r);

        free(xarr);
        free(yarr);
        //printf("div test [%d] finished\n", iter);
    }
    fclose(fp_div);
    printf("Division Cycles: %lu\n", cc);
    cc = 0;

    //! barrett test*********************************************************
    FILE* fp_bar = NULL;
    fp_bar = fopen("test/test_bar.txt", "w");
    assert(fp_bar != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {

        int ylen = (rand() % 10) + 1;
        int xlen = ylen * 2;
        bigint* r = NULL;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_barrett_reduction(&r, x, y);
        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_bar, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_bar, x, idx);
        }fprintf(fp_bar, "%s", "\n");

        fprintf(fp_bar, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_bar, y, idx);
        }fprintf(fp_bar, "%s", "\n");

        fprintf(fp_bar, "%s", "r: ");
        for (int idx = r->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_bar, r, idx);
        }fprintf(fp_bar, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&r);

        free(xarr);
        free(yarr);
        //printf("bar test [%d] finished\n", iter);
    }
    fclose(fp_bar);
    printf("Barrett Reduction Cycles: %lu\n", cc);
    cc = 0;

    //! exp test*********************************************************
    FILE* fp_exp = NULL;
    fp_exp = fopen("test/test_exp.txt", "w");
    assert(fp_exp != NULL);

    for (int iter = 0; iter < ITERNUM; iter++) {

        int xlen = (rand() % 10) + 1;
        int ylen = 1;
        xlen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand() % 10;
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_LtR(&z, &x, y);
        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_exp, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_exp, x, idx);
        }fprintf(fp_exp, "%s", "\n");

        fprintf(fp_exp, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_exp, y, idx);
        }fprintf(fp_exp, "%s", "\n");

        fprintf(fp_exp, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_exp, z, idx);
        }fprintf(fp_exp, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("exp test [%d] finished\n", iter);
    }
    fclose(fp_exp);
    printf("Exponention Cycles: %lu\n", cc);
    cc = 0;

    // void bi_mul_improvedtextbook(bigint * *dest, bigint * src1, bigint * src2)

    //! Improved multiplication test*********************************************************
    FILE* fp_impvmul = NULL;
    fp_impvmul = fopen("test/test_impvmul.txt", "w");
    assert(fp_impvmul != NULL);
    for (int iter = 0; iter < ITERNUM; iter++) {
        int xlen = (rand() % 100) + 1;
        int ylen = (rand() % 100) + 1;
        xlen = ylen = WORDLEN;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen + ylen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_mul_improvedtextbook(&z, x, y);

        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_impvmul, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_impvmul, x, idx);
        }fprintf(fp_impvmul, "%s", "\n");

        fprintf(fp_impvmul, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_impvmul, y, idx);
        }fprintf(fp_impvmul, "%s", "\n");

        fprintf(fp_impvmul, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_impvmul, z, idx);
        }fprintf(fp_impvmul, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&z);

        free(xarr);
        free(yarr);
    }
    fclose(fp_impvmul);
    printf("Improved Multiplication Cycles: %lu\n", cc);
    cc = 0;


    //! modexp test*********************************************************
    FILE* fp_modexp = NULL;
    fp_modexp = fopen("test/test_modexp.txt", "w");
    assert(fp_modexp != NULL);

    for (int iter = 0; iter < 1; iter++) {
        bigint* mod = NULL;

        int xlen = (rand() % 100) + 1;
        int ylen = rand() % 10;

        xlen = ylen = WORDLEN;
        int modlen = xlen + 1;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen);
        bi_new(&mod, modlen);

        xarr = (word*)calloc(xlen, sizeof(word));
        yarr = (word*)calloc(ylen, sizeof(word));

        for (int i = 0; i < xlen; i++) {
            xarr[i] = rand();
        }
        for (int i = 0; i < ylen; i++) {
            yarr[i] = rand();
        }

        mod->a[modlen - 1] = 0x01;      // mod = 1 << bitlen
        for (int i = modlen - 2; i >= 0; i--) {
            mod->a[i] = 0x00;
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        start = cpucycles();
        bi_LtR_mod(&z, &x, y, mod);
        end = cpucycles();
        cc += (end - start) / ITERNUM;

        fprintf(fp_modexp, "%s", "x: ");
        for (int idx = x->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_modexp, x, idx);
        }fprintf(fp_modexp, "%s", "\n");

        fprintf(fp_modexp, "%s", "y: ");
        for (int idx = y->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_modexp, y, idx);
        }fprintf(fp_modexp, "%s", "\n");

        fprintf(fp_modexp, "%s", "modulo: ");
        for (int idx = mod->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_modexp, mod, idx);
        }fprintf(fp_modexp, "%s", "\n");

        fprintf(fp_modexp, "%s", "z: ");
        for (int idx = z->wordlen - 1; idx >= 0; idx--) {
            fprint_format(fp_modexp, z, idx);
        }fprintf(fp_modexp, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&mod);
        bi_delete(&z);

        free(xarr);
        free(yarr);
        //printf("modexp test [%d] finished\n", iter);
    }
    fclose(fp_modexp);
    printf("Modular Exponention Cycles: %lu\n", cc);
    cc = 0;
    printf("\n==============================================\n\n");

#if RSA_TEST
    //! rsa test*********************************************************
    for (int iter = 0; iter < 1; iter++) {

        bigint* m = NULL;
        bigint* ct = NULL;
        bigint* pt = NULL;

        int mlen = (rand() % 10) + 1;
        bi_new(&m, mlen);

        word* marr = (word*)calloc(mlen, sizeof(word));
        for (int i = 0; i < mlen; i++) {
            marr[i] = rand() + 1;     
        }

        bi_set_by_array(&m, NONNEGATIVE, marr, mlen);

        RSA_encrypt(&ct, m);
        RSA_decrypt(&pt, ct);

        if (RSA_verify(m, pt) == -1) {
            printf("RSA failed at iter %d\n", iter);
        }
        else {
            printf("rsa test [%d] finished\n", iter);
        }

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&mod);
        bi_delete(&z);
    }
#endif

    return 0;
}
