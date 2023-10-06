#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "bigint.h"

//todo addc, multi-precision add, final add, sub

// calculate dst <- x + y
void bi_addc(bigint* dst, bigint* x, bigint* y){
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
    if(dst->wordlen <= x->wordlen){
        bi_expand(dst, x->wordlen + 1);
    }

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
}

// Perform dst <- x - y
// Assuming 0 < y <= x
void bi_subc(bigint* dst, bigint* x, bigint* y){
    int borrow = 0;

    // Expand y if y->wordlen is less than x->wordlen
    if(y->wordlen < x->wordlen){
        bi_expand(y, x->wordlen);
    }

    for(int i = 0; i < x->wordlen; i++){
        dst->a[i] = x->a[i] - y->a[i] - borrow;

        // check borrow
        if(x->a[i] < y->a[i]){
            borrow = 1;
        }
        else if((x->a[i] == y->a[i]) && (borrow == 1)){
            borrow = 1;
        }
        else{
            borrow = 0;
        }
    }

    if(borrow == 1){
        
    }

    bi_refine(dst);
}
