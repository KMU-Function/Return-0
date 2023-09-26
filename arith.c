#include <stdint.h>
#include "array.h"
#include "bigint.h"

void bi_add_sigleword(word* dst, word x, word y, int* carry){
    *carry = 0;
    *dst = x + y;
    if(*dst < x)
        *carry = 1;
}

void bi_add(bigint* dst, bigint* x, bigint* y, int* carry){
    
}