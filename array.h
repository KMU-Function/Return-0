<<<<<<< HEAD
#ifndef _ARRAY_H
#define _ARRAY_H

#include "bigint.h"

void init_array(word* a, int wordlen);
void copy_array(word* dst, word* src, int wordlen);
void gen_rand_array(word* a, int wordlen);


=======
#ifndef _ARRAY_H
#define _ARRAY_H

#include "bigint.h"

void init_array(word* a, int wordlen);
void copy_array(word* dst, word* src, size_t wordlen);
void gen_rand_array(word* a, size_t wordlen);


>>>>>>> c1eda3d0dfd7374430e5e74694168e35156d3c0a
#endif