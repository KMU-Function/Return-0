#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "bigint.h"
#include "array.h"

//todo reduction, show_bin/dec

void bi_delete(bigint** x){
    if(*x == NULL)
        return;

#ifdef ZEROIZE 
    init_array((*x)->a, (*x)->wordlen);
#endif

    if((*x)->a != NULL){
        free((*x)->a);
        (*x)->a = NULL;
    }

    free(*x);
    *x = NULL;
}

/**
* @brief Create BigInt x
*/
void bi_new(bigint** x, int wordlen) {
    assert(wordlen >= 0);

    if(*x != NULL)
        bi_delete(x);

    *x = (bigint*)calloc(1, sizeof(bigint));
    (*x)->sign = NONNEGATIVE;
    (*x)->wordlen = wordlen;

    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}



/**
* @brief Create BigInt x by array
*/
int bi_set_by_array(bigint** x, int sign, word* a, int wordlen) {
    (*x)->sign = sign;
    (*x)->wordlen = wordlen;
    for(int i = 0; i < wordlen; i++){
        (*x)->a[i] = a[i];
    }
    return 0;
}

// //todo
// /**
// * @brief Create BigInt x by string
// */
// int bi_set_by_string(bigint** x, int sign, char* str, int base) {
//     return 0;
// }

/**
* @brief Refine BigInt x (Remove Last Zero Words)
*/
void bi_refine(bigint* x) {
    if(x == NULL)
        return;

    int new_wordlen = x->wordlen;

    while (new_wordlen > 1) {
        if(x->a[new_wordlen - 1] != 0)
            break;
        new_wordlen--;
    }
    if(x->wordlen != new_wordlen){
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    }

    if((x->wordlen == 1) && (x->a[0] == 0x00))
        x->sign = NONNEGATIVE;
}

/**
* @brief Assign BigInt x to y (Y <- X)
*/
void bi_assign(bigint** y, bigint* x){
    if(*y != NULL){
        bi_delete(y);
    }
    if(*y == x){
        return;
    }

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    copy_array((*y)->a, x->a, x->wordlen);
}

/**
* @brief Expand BigInt x with new word length by zero
*/
void bi_expand(bigint* x, int new_wordlen){
    if(x->wordlen > new_wordlen){
        fprintf(stderr, "wordlen is larger than new wordlen\n");
        return;
    }

    if(x->wordlen == new_wordlen)
        return;

    int old_wordlen = x->wordlen;
    x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    x->wordlen = new_wordlen;

    // initialize reallocates memory into zero
    for(int i = old_wordlen; i < x->wordlen; i++){
        x->a[i] = 0;
    }
}

/**
* @brief Generate random BigInt x
*/
void bi_gen_rand(bigint** x, int sign, int wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    gen_rand_array((*x)->a, (*x)->wordlen);     // get random data
    bi_refine(*x);
}

/**
* @brief Create BigInt x and initialize its value to 0x1
*/
void bi_set_one(bigint** x) {
    bi_new(x, 1);
    (*x)->sign = NONNEGATIVE;
    (*x)->a[0] = 0x1;
}

/**
* @brief Create BigInt x and initialize its value to 0x0
*/
void bi_set_zero(bigint** x) {
    bi_new(x, 1);
    (*x)->sign = NONNEGATIVE;
    (*x)->a[0] = 0x0;
}

/**
* @brief Check if value of BigInt x is 0x1
* return 1(true) if BigInt x is 0x1, 0(false) otherwise
*/
int bi_is_one(bigint* x) {
    if(x == NULL)
        return false;
            
    else if(x->wordlen == 0)
        return false;

    else if(x->sign != NONNEGATIVE || x->a[0] != 1)
        return false;

    for(int i = 0; i < x->wordlen - 1; i++){
        if(x->a[i] != 0)
            return false;
    }
    return true;
}

/**
* @brief Check if value of BigInt x is 0x0
* return 1(true) if BigInt x is 0x0, 0(false) otherwise
*/
int bi_is_zero(bigint* x) {
    if(x == NULL)
        return 0;
    
    else if(x->wordlen == 0)
        return 0;

    else if(x->sign != NONNEGATIVE || x->a[0] != 0)
        return 0;

    for(int i = x->wordlen; i >= 0; i--){
        if(x->a[i] != 0)
            return 0;
    }
    return 1;
}

/**
* @brief Compare absolute value of positive BigInt x and y
* return 1 if x > y
* return 0 if x = y
* return -1 if x < y
*/
int compare_abs(bigint* x, bigint* y){
    if(x->wordlen > y->wordlen)
        return 1;
    else if (x-> wordlen < y->wordlen)
        return -1;
    for(int i = x->wordlen - 1; i >= 0; i++){
        if(x->a[i] > y->a[i])
            return 1;
        else if(x->a[i] < y->a[i])
            return -1;
    }
    return 0;
}

/**
* @brief Flip sign bit of BigInt x
*/
void bi_flip_sign(bigint* x) {
    if(x->sign == NONNEGATIVE){
        x->sign = NEGATIVE;
    }
    else{
        x->sign = NONNEGATIVE;
    }
}

/**
* @brief Compare value of BigInt x and y
* return 1 if x > y
* return 0 if x = y
* return -1 if x < y
*/
int compare(bigint* x, bigint* y){
    if ((x)->sign == NONNEGATIVE && (y)->sign == NEGATIVE)
      return 1; // bi_X는 양수, bi_Y는 음수일 경우 반환은 First is Big
   if ((x)->sign == NEGATIVE && (y)->sign == NONNEGATIVE)
      return -1;                                               // 반대일 경우, 반환은 Second is big
   if ((x)->sign == NONNEGATIVE && (y)->sign == NONNEGATIVE) // 비교하려는 두수의 부호가 동일한 경우
   {
      if ((x)->wordlen > ((y)->wordlen))
         return 1; // Wordlen 비교
      if ((x)->wordlen < ((y)->wordlen))
         return -1; // Wordlen 비교
      int cnt_i;
      for (cnt_i = (x)->wordlen - 1; cnt_i >= 0; cnt_i--)
      {
         if ((x)->a[cnt_i] > (y)->a[cnt_i]) // 두수의 wordlen이 같을때 배열의 각 word를 비교하기
            return 1;                      // First is big (bi_X>bi_Y)
         if ((x)->a[cnt_i] < (y)->a[cnt_i])
            return -1; // Second is big
      }
      return 0; // 만약 해당하는것이 아무것도 없는경우 두수는 같다고 표현
   }
   if ((x)->sign == NEGATIVE && (y)->sign == NEGATIVE) // 바로 위의 if Condition과 같은 논리이다.
   {
      if ((x)->wordlen > ((y)->wordlen))
         return -1;
      if ((x)->wordlen < ((y)->wordlen))
         return 1;
      int cnt_i;
      for (cnt_i = (x)->wordlen - 1; cnt_i >= 0; cnt_i--)
      {
         if ((x)->a[cnt_i] > (y)->a[cnt_i])
            return -1;
         if ((x)->a[cnt_i] < (y)->a[cnt_i])
            return 1;
      }
      return 0;
   }

   abort();
}

/**
* @brief Get word length of BigInt x
*/
int get_wordlen(bigint* x) {
    return x->wordlen;
}

/**
* @brief Get bit length of BigInt x
*/
int get_bitlen(bigint* x) {
    return x->wordlen * sizeof(word) * 8;
}

/**
* @brief Get i-th bit of BigInt x
* @param i index of the bit to be extracted
*/
int get_ith_bit(bigint* x, int i) {
    return (x->a[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8))) & 0x1;
}

/**
* @brief Get sign bit of BigInt x
*/
int get_sign(bigint* x) {
    return x->sign;
}

/**
* @brief Get flipped sign bit of BigInt x
*/
int get_filpped_sign(bigint* x) {
    if(x->sign == NONNEGATIVE)
        return NEGATIVE;
    else
        return NONNEGATIVE;
}

/**
* @brief Print BigInt x in hexa representation
*/
void bi_show_hex(bigint* x){
    for(int i = 0; i < x->wordlen; i++){
#if DTYPE == 8
        printf("%02x ", x->a[i]);
#elif DTYPE == 32
        printf("%08x ", x->a[i]);
#elif DTYPE == 64
        printf("%016llx ", x->a[i]);
#else
#error "UNSUPPORTED TYPE\n";
#endif
    }printf("\n");
}

/**
* @brief Print BigInt x in hexadecimal representation in order
*/
void bi_show_hex_inorder(bigint* x) {
    for(int i = x->wordlen - 1; i >= 0; i--){
#if DTYPE == 8
        printf("%02x ", x->a[i]);
#elif DTYPE == 32
        printf("%08x ", x->a[i]);
#elif DTYPE == 64
        printf("%016llx ", x->a[i]);
#else
#error "UNSUPPORTED TYPE\n";
#endif
    }printf("\n");
}

// 이대로 둬야 할 듯 (테스트 통과)
void bi_shift_right_word(bigint** x, int bytelen) {
    for(int i = 0; i < (*x)->wordlen - bytelen; i++){
        (*x)->a[i] = (*x)->a[i + bytelen];
    }

    for(int i = (*x)->wordlen - bytelen; i < (*x)->wordlen; i++){
        (*x)->a[i] = 0;
    }

    bi_refine(*x);
}

void bi_shift_left_word(bigint** x, int bytelen) {
    if(bytelen == 0){
        return;
    }

    bi_expand(*x, (*x)->wordlen + bytelen);

    for(int i = (*x)->wordlen - 1; i >= bytelen; i--){
        (*x)->a[i] = (*x)->a[i - bytelen];
    }
    

    for(int i = bytelen - 1; i >= 0; i--){
        (*x)->a[i] = 0;
    }
}

/**
* @brief Bitwise shift right
* @param x Pointer of a big integer
* @param r Shift amount in bit
*/
void bi_shr(bigint** src, int r) {
    // Check for invalid inputs: NULL source or non-positive shift value.
    if (*src == NULL || r < 0) {
        printf("Invalid inputs. Function: Right_shift\n");
        return;
    }
    
    // Calculate word and bit offsets for the shift operation.
    int word_shift = r / (8 * sizeof(word)); // Offset in words
    int bit_offset = r % (8 * sizeof(word));  // Offset in bits
    
    // Create a temporary bigint to hold the shifted value.
    bigint* temp = NULL;
    bi_new(&temp, (*src)->wordlen - word_shift );
    if (word_shift >= (*src)->wordlen) {
        bi_set_zero(src);
        return;
    }
    // Perform word shift.
    for (int i = 0; i < temp->wordlen; i++) {
        int shifted_index = i + word_shift;
        temp->a[i] = (*src)->a[shifted_index];
    }
       
    // Perform bit shift within each word.
    if (bit_offset > 0) {
        word carry = 0;
        for (int i = temp->wordlen - 1; i >= 0; i--) {
            word temp_word = temp->a[i];
            word shifted_word = temp->a[i] >> bit_offset;
            temp->a[i] = (shifted_word | carry);
            carry = temp_word << ((8 * sizeof(word)) - bit_offset);
        }
    }

    bi_refine(temp);
    bi_new(src, temp->wordlen);
    memcpy((*src)->a, temp->a, sizeof(word) * temp->wordlen);
    bi_delete(&temp);
}


void bi_shl(bigint** x, int r) {
    // Check for invalid inputs: NULL source or non-positive shift value.
    if (*x == NULL || r == 0) {
        return;
    }
    
    //printf("r = %d\n", r);
    //bi_show_hex_inorder(*x);

    // Calculate word and bit offsets for the shift operation.
    int word_shift = r / (8 * sizeof(word)); // Offset in words
    int bit_offset = r % (8 * sizeof(word));  // Offset in bits

    // Create a temporary bigint to hold the shifted value.
    bigint* temp = NULL;  // Changed to bigint* from bigint**
    bi_new(&temp, (*x)->wordlen + word_shift + 1);  // Pass the address of temp

    // Perform word shift.
    if (word_shift == 0) {
        memcpy(temp->a, (*x)->a, sizeof(word) * (*x)->wordlen);
    }
    else {
        for (int i = 0; i < (*x)->wordlen; i++) {
            int shifted_index = i + word_shift;
            temp->a[shifted_index] = (*x)->a[i];
        }
    }

    word carry = 0;
    if(bit_offset != 0){
        for (int i = 0; i < temp->wordlen; i++) {
            word temp_word = temp->a[i];
            word shifted_word = temp->a[i] << bit_offset;
            temp->a[i] = (shifted_word | carry);
            carry = temp_word >> ((8 * sizeof(word)) - bit_offset);
        }

        // Handle the final carry.
        if (temp->wordlen > 0 && carry > 0) {
            temp->a[temp->wordlen - 1] |= carry;
        }
    }


    bi_refine(temp);
    bi_assign(x, temp);
    // bi_new(x, temp->wordlen);
    // memcpy((*x)->a, temp->a, sizeof(word) * temp->wordlen);
    bi_delete(&temp);  // Pass the address of temp to bi_delete
}

void bi_set_min_words(bigint **x, int sign, size_t wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    (*x)->a[wordlen-1] = 1;
}


//todo
/**
* @brief Print BigInt x in binary representation in order
*/
// void bi_show_bin(bigint* x){}
// void bi_show_bin_inorder(bigint* x){}
