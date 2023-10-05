<<<<<<< HEAD
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bigint.h"
#include "rng.h"

void init_array(word* a, int wordlen){
    for(int i = 0; i < wordlen; i++){
        a[i] = 0;
    }
}

void copy_array(word* dst, word* src, int wordlen){
    for(int i = 0; i < wordlen; i++){
        dst[i] = src[i];
    }
}

void gen_rand_array(word* a, int wordlen){
    for(int i = 0; i < wordlen; i++){
        uint8_t* arr = (uint8_t*)calloc(wordlen * sizeof(int), sizeof(uint8_t));
        randombytes(arr, wordlen * sizeof(int));
        copy_array(a, (word*)arr, wordlen);
    }
=======
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bigint.h"
#include "rng.h"

void init_array(word* a, size_t wordlen){
    for(int i = 0; i < wordlen; i++){
        a[i] = 0;
    }
}

void copy_array(word* dst, word* src, size_t wordlen){
    for(int i = 0; i < wordlen; i++){
        dst[i] = src[i];
    }
}

void gen_rand_array(word* a, size_t wordlen){
    for(int i = 0; i < wordlen; i++){
        uint8_t* arr = (uint8_t*)calloc(wordlen * sizeof(int), sizeof(uint8_t));
        randombytes(arr, wordlen * sizeof(int));
        copy_array(a, (word*)arr, wordlen);
    }
>>>>>>> c1eda3d0dfd7374430e5e74694168e35156d3c0a
}