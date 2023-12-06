/**
 * @file array.c
 * @brief source file for big integer array basic operations.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bigint.h"
#include "rng.h"

 /**
  * @brief Initialize an array with zero values.
  * @param a Pointer to the array to be initialized.
  * @param wordlen The length of the array.
  */
void init_array(word* a, int wordlen) {
    if (a == NULL || wordlen == 0)
        return;
    for (int i = 0; i < wordlen; i++) {
        a[i] = 0;
    }
}

/**
 * @brief Copy elements from the source array to the destination array.
 * @param dst Pointer to the destination array.
 * @param src Pointer to the source array.
 * @param wordlen The length of the arrays.
 */
void copy_array(word* dst, word* src, int wordlen) {
    for (int i = 0; i < wordlen; i++) {
        dst[i] = src[i];
    }
}

/**
 * @brief Generate a random array with a specified word length using random data.
 * @param a Pointer to the array to store the random values.
 * @param wordlen The length of the array.
 * @details This function generates random data using the `randombytes` function and copies it to the specified array.
 *          Ensure that the `randombytes` function is available and properly implemented in your code.
 */
void gen_rand_array(word* a, int wordlen) {
    for (int i = 0; i < wordlen; i++) {
        uint8_t* arr = (uint8_t*)calloc(wordlen * sizeof(int), sizeof(uint8_t));
        randombytes(arr, wordlen * sizeof(int));
        copy_array(a, (word*)arr, wordlen);
    }
}
