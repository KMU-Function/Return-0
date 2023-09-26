#ifndef _BIGINT_H
#define _BIGINT_H

#include <stdint.h>

#define NONNEGATIVE 0
#define NEGATIVE    1

typedef uint32_t word;

typedef struct _BIGINT{
    int sign ;      // NEGATIVE or NON - NEGATIVE
    int wordlen ;   // wordlen >= 0
    word * a;       // address for big integer
} bigint;

void bn_delete(bigint** x);
void bn_new(bigint** x, int wordlen);

#endif