/**
 * @file rsa.h
 * @brief header file for RSA cryptography function using big integer.
 */

#ifndef _RSA_H
#define _RSA_H

#include "bigint.h"
#include "arith.h"


int RSA_encrypt(bigint** ct, bigint* pt);
int RSA_decrypt(bigint** pt, bigint* ct);
int RSA_verify(bigint* ct, bigint* pt);

#endif
