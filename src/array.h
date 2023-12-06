/**
 * @file array.h
 * @brief Header file for big integer array basic operations.
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include "bigint.h"

void init_array(word* a, int wordlen);
void copy_array(word* dst, word* src, int wordlen);
void gen_rand_array(word* a, int wordlen);


#endif
