#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bigint.h"
#include "rng.h"

/**
* @brief Initialize array with zero values
*/
void init_array(word* a, int wordlen) {
    for(int i = 0; i < wordlen; i++){
        a[i] = 0;
    }
}

/**
* @brief Copy array from source to destination 
*/
void copy_array(word* dst, word* src, int wordlen) {
    for(int i = 0; i < wordlen; i++){
        dst[i] = src[i];
    }
}

/**
* @brief Generate random array with a specified word length using random data 
*/
void gen_rand_array(word* a, int wordlen) {
    for(int i = 0; i < wordlen; i++){
        uint8_t* arr = (uint8_t*)calloc(wordlen * sizeof(int), sizeof(uint8_t));
        randombytes(arr, wordlen * sizeof(int));
        copy_array(a, (word*)arr, wordlen);
    }
}
