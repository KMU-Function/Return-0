/**
 * @file bigint.c
 * @brief Source file for big integer basic operations.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "bigint.h"
#include "array.h"


 /**
  * @brief Deallocate memory associated with a BigInt and release resources.
  * @param x Pointer to the BigInt pointer. After calling this function, the pointer will be set to NULL.
  * @return None.
  */
void bi_delete(bigint** x) {
    if (*x == NULL)
        return;

#ifdef ZEROIZE 
    init_array((*x)->a, (*x)->wordlen);
#endif

    if ((*x)->a != NULL) {
        free((*x)->a);
        (*x)->a = NULL;
    }

    free(*x);
    *x = NULL;
}

/**
 * @brief Create a new BigInt with a specified word length.
 * @param x Pointer to the BigInt pointer where the new BigInt will be stored.
 * @param wordlen The word length of the BigInt.
 * @return None.
 */
void bi_new(bigint** x, int wordlen) {
    assert(wordlen >= 0);

    if (*x != NULL)
        bi_delete(x);

    *x = (bigint*)calloc(1, sizeof(bigint));
    (*x)->sign = NONNEGATIVE;
    (*x)->wordlen = wordlen;

    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}

/**
 * @brief Create BigInt x by array.
 * @param x Pointer to the BigInt pointer.
 * @param sign Sign of the BigInt.
 * @param a Array containing the BigInt data.
 * @param wordlen Word length of the BigInt.
 * @return 0 on success.
 */
int bi_set_by_array(bigint** x, int sign, word* a, int wordlen) {
    (*x)->sign = sign;
    (*x)->wordlen = wordlen;
    for (int i = 0; i < wordlen; i++) {
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
 * @brief Refine BigInt x by removing trailing zero words.
 * @param x Pointer to the BigInt to be refined.
 */
void bi_refine(bigint* x) {
    if (x == NULL)
        return;

    int new_wordlen = x->wordlen;

    while (new_wordlen > 1) {
        if (x->a[new_wordlen - 1] != 0)
            break;
        new_wordlen--;
    }
    if (x->wordlen != new_wordlen) {
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    }

    if ((x->wordlen == 1) && (x->a[0] == 0x00))
        x->sign = NONNEGATIVE;
}

/**
 * @brief Assign the value of BigInt x to BigInt y (Y <- X).
 * @param y Pointer to the destination BigInt where the value of x will be assigned.
 * @param x BigInt to be assigned to y.
 */
void bi_assign(bigint** y, bigint* x) {
    if (*y != NULL) {
        bi_delete(y);
    }
    if (*y == x) {
        return;
    }

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    copy_array((*y)->a, x->a, x->wordlen);
}

/**
 * @brief Expand the BigInt x with a new word length, padding with zeros.
 * @param x Pointer to the BigInt to be expanded.
 * @param new_wordlen New word length for the BigInt.
 */
void bi_expand(bigint* x, int new_wordlen) {
    if (x->wordlen > new_wordlen) {
        fprintf(stderr, "wordlen is larger than new wordlen\n");
        return;
    }

    if (x->wordlen == new_wordlen)
        return;

    int old_wordlen = x->wordlen;
    x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    x->wordlen = new_wordlen;

    // initialize reallocates memory into zero
    for (int i = old_wordlen; i < x->wordlen; i++) {
        x->a[i] = 0;
    }
}

/**
 * @brief Generate a random BigInt x with the specified sign and word length.
 * @param x Pointer to the destination BigInt where the random value will be stored.
 * @param sign Sign of the BigInt (NONNEGATIVE or NEGATIVE).
 * @param wordlen Word length of the BigInt.
 */
void bi_gen_rand(bigint** x, int sign, int wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    gen_rand_array((*x)->a, (*x)->wordlen);     // get random data
    bi_refine(*x);
}

/**
 * @brief Create a BigInt x and initialize its value to 0x1.
 * @param x Pointer to the BigInt to be created.
 */
void bi_set_one(bigint** x) {
    bi_new(x, 1);
    (*x)->sign = NONNEGATIVE;
    (*x)->a[0] = 0x1;
}

/**
 * @brief Create a BigInt x and initialize its value to 0x0.
 * @param x Pointer to the BigInt to be created.
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
    if (x == NULL)
        return false;

    else if (x->wordlen == 0)
        return false;

    else if (x->sign != NONNEGATIVE || x->a[0] != 1)
        return false;

    for (int i = 0; i < x->wordlen - 1; i++) {
        if (x->a[i] != 0)
            return false;
    }
    return true;
}

/**
 * @brief Check if the value of BigInt x is equal to 0x0.
 * @param x BigInt to be checked.
 * @return 1 (true) if BigInt x is equal to 0x0, 0 (false) otherwise.
 */
int bi_is_zero(bigint* x) {
    if (x == NULL)
        return 0;

    else if (x->wordlen == 0)
        return 0;

    else if (x->sign != NONNEGATIVE || x->a[0] != 0)
        return 0;

    for (int i = x->wordlen - 1; i >= 0; i--) {
        if (x->a[i] != 0)
            return 0;
    }
    return 1;
}

/**
 * @brief Compare the absolute values of positive BigInt x and y.
 * @param x Pointer to the first BigInt.
 * @param y Pointer to the second BigInt.
 * @return 1 if x > y,
 *         0 if x = y,
 *        -1 if x < y.
 */
int compare_abs(bigint* x, bigint* y) {
    if (x->wordlen > y->wordlen)
        return 1;
    else if (x->wordlen < y->wordlen)
        return -1;
    for (int i = x->wordlen - 1; i >= 0; i++) {
        if (x->a[i] > y->a[i])
            return 1;
        else if (x->a[i] < y->a[i])
            return -1;
    }
    return 0;
}

/**
 * @brief Flip the sign bit of BigInt x.
 * @param x BigInt whose sign bit will be flipped.
 */
void bi_flip_sign(bigint* x) {
    if (x->sign == NONNEGATIVE) {
        x->sign = NEGATIVE;
    }
    else {
        x->sign = NONNEGATIVE;
    }
}

/**
 * @brief Compare the values of BigInt x and y.
 * @param x BigInt for comparison.
 * @param y BigInt for comparison.
 * @return  1 if x > y,
 *          0 if x = y,
 *         -1 if x < y.
 */
int compare(bigint* x, bigint* y) {
    // Case: x is nonnegative and y is negative
    if ((x)->sign == NONNEGATIVE && (y)->sign == NEGATIVE)
        return 1; // x is greater 

    // Case: x is negative and y is nonnegative
    if ((x)->sign == NEGATIVE && (y)->sign == NONNEGATIVE)
        return -1; // y is greater 

    // Case: Both x and y are nonnegative
    if ((x)->sign == NONNEGATIVE && (y)->sign == NONNEGATIVE) {
        // Compare word lengths
        if ((x)->wordlen > ((y)->wordlen))
            return 1; // x is greater
        if ((x)->wordlen < ((y)->wordlen))
            return -1; // y is greater

        // Compare individual words from the most significant to least significant
        int cnt_i;
        for (cnt_i = (x)->wordlen - 1; cnt_i >= 0; cnt_i--) {
            if ((x)->a[cnt_i] > (y)->a[cnt_i])
                return 1; // x is greater
            if ((x)->a[cnt_i] < (y)->a[cnt_i])
                return -1; // y is greater
        }
        return 0; // If no differences found, the numbers are equal
    }

    // Case: Both x and y are negative
    if ((x)->sign == NEGATIVE && (y)->sign == NEGATIVE) {
        // Compare word lengths
        if ((x)->wordlen > ((y)->wordlen))
            return -1; // y is greater
        if ((x)->wordlen < ((y)->wordlen))
            return 1; // x is greater

        // Compare individual words from the most significant to least significant
        int cnt_i;
        for (cnt_i = (x)->wordlen - 1; cnt_i >= 0; cnt_i--) {
            if ((x)->a[cnt_i] > (y)->a[cnt_i])
                return -1; // y is greater
            if ((x)->a[cnt_i] < (y)->a[cnt_i])
                return 1; // x is greater
        }
        return 0; // If no differences found, the numbers are equal
    }

    // Unreachable code (abort if none of the above cases matched)
    abort();
}

/**
 * @brief Get the word length of a BigInt.
 * This function retrieves the number of words used to represent the BigInt.
 * @param x Pointer to the BigInt.
 * @return The word length of the BigInt.
 */
int get_wordlen(bigint* x) {
    return x->wordlen;
}

/**
 * @brief Get the bit length of a BigInt.
 * This function calculates and returns the total number of bits used to represent the BigInt.
 * @param x Pointer to the BigInt.
 * @return The bit length of the BigInt.
 */
int get_bitlen(bigint* x) {
    return x->wordlen * sizeof(word) * 8;
}

/**
 * @brief Get the i-th bit of a BigInt.
 * This function retrieves the value of the specified bit at position i.
 * @param x Pointer to the BigInt.
 * @param i Index of the bit to be extracted.
 * @return The value of the i-th bit (0 or 1).
 */
int get_ith_bit(bigint* x, int i) {
    return (x->a[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8))) & 0x1;
}

/**
 * @brief Get the sign bit of a BigInt.
 * This function retrieves the sign bit of the BigInt.
 * @param x Pointer to the BigInt.
 * @return The sign bit (NONNEGATIVE or NEGATIVE).
 */
int get_sign(bigint* x) {
    return x->sign;
}

/**
 * @brief Get the flipped sign bit of a BigInt.
 * This function retrieves the opposite sign bit of the BigInt.
 * @param x Pointer to the BigInt.
 * @return The flipped sign bit (NEGATIVE if the original is NONNEGATIVE, and vice versa).
 */
int get_filpped_sign(bigint* x) {
    if (x->sign == NONNEGATIVE)
        return NEGATIVE;
    else
        return NONNEGATIVE;
}

/**
 * @brief Display the hexadecimal representation of a BigInt.
 * @param x Pointer to the BigInt to be displayed.
 */
void bi_show_hex(bigint* x) {
    for (int i = 0; i < x->wordlen; i++) {
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
 * @brief Display the hexadecimal representation of a BigInt in reverse order.
 * @param x Pointer to the BigInt to be displayed.
 */
void bi_show_hex_inorder(bigint* x) {
    for (int i = x->wordlen - 1; i >= 0; i--) {
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
 * @brief Shift the BigInt to the right by a specified number of words.
 * @param x Pointer to the BigInt to be shifted.
 * @param bytelen Number of words to shift the BigInt to the right.
 */
void bi_shift_right_word(bigint** x, int bytelen) {
    for (int i = 0; i < (*x)->wordlen - bytelen; i++) {
        (*x)->a[i] = (*x)->a[i + bytelen];
    }

    for (int i = (*x)->wordlen - bytelen; i < (*x)->wordlen; i++) {
        (*x)->a[i] = 0;
    }

    bi_refine(*x);
}

/**
 * @brief Shift the BigInt to the left by a specified number of words.
 * @param x Pointer to the BigInt to be shifted.
 * @param bytelen Number of words to shift the BigInt to the left.
 */
void bi_shift_left_word(bigint** x, int bytelen) {
    if (bytelen == 0) {
        return;
    }

    bi_expand(*x, (*x)->wordlen + bytelen);

    for (int i = (*x)->wordlen - 1; i >= bytelen; i--) {
        (*x)->a[i] = (*x)->a[i - bytelen];
    }


    for (int i = bytelen - 1; i >= 0; i--) {
        (*x)->a[i] = 0;
    }
}

/**
 * @brief Perform a bitwise shift to the right on a BigInt.
 * @param x Pointer to the BigInt to be shifted.
 * @param r Shift amount in bits.
 */
void bi_shr(bigint** x, int r) {
    // Check for invalid inputs: NULL source or non-positive shift value.
    if (*x == NULL || r < 0) {
        return;
    }

    // Calculate word and bit offsets for the shift operation.
    int word_shift = r / (8 * sizeof(word)); // Offset in words
    int bit_offset = r % (8 * sizeof(word));  // Offset in bits

    // check x become zero
    if((*x)->wordlen - word_shift <= 0){
        bi_set_zero(x);
        return;
    }

    // Create a temporary bigint to hold the shifted value.
    bigint* temp = NULL;
    bi_new(&temp, (*x)->wordlen - word_shift);
    if (word_shift >= (*x)->wordlen) {
        bi_set_zero(x);
        return;
    }
    // Perform word shift.
    for (int i = 0; i < temp->wordlen; i++) {
        int shifted_index = i + word_shift;
        temp->a[i] = (*x)->a[shifted_index];
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
    bi_new(x, temp->wordlen);
    memcpy((*x)->a, temp->a, sizeof(word) * temp->wordlen);
    bi_delete(&temp);
}

/**
 * @brief Perform a bitwise shift to the left on a BigInt.
 * @param x Pointer to the BigInt to be shifted.
 * @param r Shift amount in bits.
 */
void bi_shl(bigint** x, int r) {
    // Check for invalid inputs: NULL source or non-positive shift value.
    if (*x == NULL || r == 0) {
        return;
    }

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
    if (bit_offset != 0) {
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
    bi_delete(&temp);  // Pass the address of temp to bi_delete
}

/**
 * @brief Set a BigInt to the minimum value with a specified word length.
 * @param x Pointer to the BigInt to be set.
 * @param sign Sign of the BigInt (NONNEGATIVE or NEGATIVE).
 * @param wordlen Word length of the BigInt.
 */
void bi_set_min_words(bigint** x, int sign, size_t wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    (*x)->a[wordlen - 1] = 1;
}

// void bi_show_bin(bigint* x){}
// void bi_show_bin_inorder(bigint* x){}
