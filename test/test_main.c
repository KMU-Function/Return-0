#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../src/api.h"

void fprint_format(FILE* fp, bigint* x, int idx){
    #if DTYPE == 8
        fprintf(fp, "%02x ", x->a[idx]);
    #elif DTYPE == 32
        fprintf(fp, "%08x ", x->a[idx]);
    #elif DTYPE == 64
        fprintf(fp, "%016x ", x->a[idx]);
    #endif
}

int main(){
    srand(time(NULL));

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word* xarr = NULL;
    word* yarr = NULL;

    //! add test*********************************************************
    FILE* fp_add = NULL;
    // fp_add = fopen("test/test_add.txt", "w");
    fp_add = fopen("../test/test_add.txt", "w");
    assert(fp_add != NULL);
    for (int iter = 0; iter < 10000; iter++) {

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

        bi_add(&z, x, x);

        fprintf(fp_add, "%s", "x: ");
        for(int idx = 0; idx < x->wordlen; idx++){
            fprint_format(fp_add, x, idx);
        }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "y: ");
        for(int idx = 0; idx < x->wordlen; idx++){
            fprint_format(fp_add, x, idx);
        }fprintf(fp_add, "%s", "\n");

        // fprintf(fp_add, "%s", "y: ");
        // for(int idx = 0; idx < y->wordlen; idx++){
        //     fprint_format(fp_add, y, idx);
        // }fprintf(fp_add, "%s", "\n");

        fprintf(fp_add, "%s", "z: ");
        for(int idx = 0; idx < z->wordlen; idx++){
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
    fp_sub = fopen("../test/test_sub.txt", "w");
    assert(fp_sub != NULL);
    
    for (int iter = 0; iter < 10000; iter++) {

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
        for(int idx = 0; idx < x->wordlen; idx++){
            fprint_format(fp_sub, x, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "y: ");
        for(int idx = 0; idx < y->wordlen; idx++){
            fprint_format(fp_sub, y, idx);
        }fprintf(fp_sub, "%s", "\n");

        fprintf(fp_sub, "%s", "z: ");
        for(int idx = 0; idx < z->wordlen; idx++){
            fprint_format(fp_sub, z, idx);
        }fprintf(fp_sub, "%s", "\n\n");

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);

        free(xarr);
        free(yarr);
    }
    fclose(fp_sub);

    // //! mul test*********************************************************
    // word w1 = rand();
    // word w2 = rand();
    // word result[2] = { 0x00 };

    // printf("%u %8X\n", w1, w1);
    // printf("%u %8X\n", w2, w2);

    // bi_mulc(result, w1, w2);

    // for (int i = 1; i >= 0; i--) {
    //     printf("%8X ", result[i]);

    // }

    return 0;
}
