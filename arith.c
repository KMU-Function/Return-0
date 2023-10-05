<<<<<<< HEAD
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "bigint.h"

//todo addc, multi-precision add, final add, sub

void bi_addc(bigint* dst, bigint* x, bigint* y){
    // expand smaller one
    if(x->wordlen < y->wordlen){
        bi_expand(x, y->wordlen);
    }
    else if(y->wordlen < x->wordlen){
        bi_expand(y, x->wordlen);
    }

    // if dst->wordlen is smaller than x or y, then realloc dst->wordlen
    // check for only x because now x->wordlen and y->wordlen are the same
    if(dst->wordlen <= x->wordlen){
        bi_expand(dst, x->wordlen + 1);
    }

    int carry = 0;
    for(int i = 0; i < x->wordlen; i++){
        dst->a[i] = x->a[i] + y->a[i] + carry;
        // check carry
        if(dst->a[i] < x->a[i]){
            carry = 1;
        }
        else{
            carry = 0;
        }
    }

    if(carry == 1){
        dst->a[dst->wordlen - 1] = 1;
    }

    bi_refine(dst);
=======
#include <stdint.h>
#include "array.h"
#include "bigint.h"

//todo addc, multi-precision add, final add, sub

void bi_add_sigleword(word* dst, word x, word y, int* carry){
    *carry = 0;
    *dst = x + y;
    if(*dst < x)
        *carry = 1;
}

void bi_addc(bigint* dst, bigint* x, bigint* y, int* carry){
    carry = 0;
    
>>>>>>> c1eda3d0dfd7374430e5e74694168e35156d3c0a
}