#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "api.h"

int main(){
    srand(time(NULL));

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word xarr[10];
    word yarr[10];



    for(int iter = 0; iter < 100; iter++){

        int xlen = rand() % 9 + 1;
        int ylen = rand() % 9 + 1;

        bi_new(&x, xlen);
        bi_new(&y, ylen);
        bi_new(&z, xlen < ylen ? ylen : xlen);

        for(int i = 0; i < xlen; i++){
            xarr[i] = rand();
        }
        for(int i = 0; i < ylen; i++){
            yarr[i] = rand();
        }

        bi_set_by_array(&x, NONNEGATIVE, xarr, xlen);
        bi_set_by_array(&y, NONNEGATIVE, yarr, ylen);

        printf("x: "); bi_show_hex(x);
        printf("y: "); bi_show_hex(y);

        bi_addc(z, x, y);

        printf("z: "); bi_show_hex(z);
        printf("\n");
    }

    return 0;
}
