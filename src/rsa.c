/**
 * @file rsa.c
 * @brief source file for RSA cryptography function using big integer.
 */

#include <stdio.h>
#include "rsa.h"

 //! Caution: prime p and q are hard-coded

#if DTYPE==8
#define plen    192
#define qlen    192
#define elen    4
#define dlen    384
#define nlen    384
#define philen  384
#elif DTYPE == 32
#define plen    48
#define qlen    48
#define elen    1
#define dlen    96
#define nlen    96
#define philen  96
#elif DTYPE==64
#define plen    24
#define qlen    24
#define elen    1
#define dlen    48
#define nlen    48
#define philen  48
#endif

/**
 * @brief Function to retrieve RSA parameters.
 * @param p Pointer to the prime number p.
 * @param q Pointer to the prime number q.
 * @param phi Pointer to the Euler's totient function result (phi(n)).
 * @param e Pointer to the public exponent e.
 * @param d Pointer to the private exponent d.
 * @param n Pointer to the modulus n.
 */
void get_RSA_param(bigint** p, bigint** q, bigint** phi, bigint** e, bigint** d, bigint** n) {
#if DTYPE==8
    static word parr[] = { 0x13, 0x7f, 0xab, 0x4e, 0xff, 0xbd, 0x42, 0xc1, 0x34, 0x90, 0x87, 0x82, 0x4a, 0x0d, 0x3c, 0x5e, 0x19, 0x3d, 0xad, 0xd9, 0xac, 0x57, 0x81, 0xd6, 0xa2, 0xb5, 0x37, 0x09, 0x13, 0x92, 0xf3, 0x24, 0xb7, 0xad, 0x94, 0xe7, 0x12, 0x8e, 0x31, 0x75, 0x42, 0x19, 0xf9, 0x84, 0xa7, 0x54, 0xdb, 0x00, 0x3f, 0xe6, 0xd7, 0xcf, 0x9b, 0x9c, 0x25, 0x51, 0x9d, 0xad, 0xc3, 0x24, 0x82, 0x67, 0xdd, 0xad, 0xd8, 0x8f, 0x95, 0xf4, 0x27, 0xed, 0xab, 0x12, 0xc9, 0x79, 0xeb, 0x64, 0x01, 0x65, 0x51, 0x2f, 0x1a, 0xd8, 0xbe, 0x87, 0x54, 0x41, 0x28, 0x5b, 0x60, 0x84, 0x88, 0x0b, 0x31, 0xa6, 0x03, 0x67, 0x1d, 0x15, 0x94, 0xe7, 0x2f, 0x27, 0x75, 0x84, 0x3d, 0x40, 0x7c, 0xb4, 0xc1, 0x89, 0x62, 0xa6, 0x4c, 0x35, 0xf5, 0xf5, 0x99, 0x69, 0x8a, 0xea, 0x7b, 0x9a, 0x60, 0xe8, 0xac, 0xb4, 0x9a, 0x49, 0xa6, 0x98, 0xbc, 0xa0, 0x17, 0x21, 0xef, 0xe1, 0x65, 0x37, 0xb7, 0x99, 0x25, 0xa3, 0xa5, 0x14, 0xd9, 0xe7, 0xe7, 0x51, 0x02, 0x89, 0x41, 0xa2, 0x58, 0x5e, 0xcd, 0xcc, 0xad, 0x0f, 0x34, 0xe3, 0xbf, 0x03, 0x86, 0x20, 0xef, 0x49, 0x98, 0x46, 0xde, 0x55, 0x66, 0xf3, 0x5c, 0xa0, 0x4c, 0x3c, 0xd1, 0xfd, 0x06, 0xf2, 0x47, 0xfe, 0xe1, 0x9c, 0xed, 0x31, 0x7a, 0x4a, 0x33, 0xc9, 0x51, 0x97 };
    static word qarr[] = { 0x97, 0xd0, 0xb6, 0x3d, 0x82, 0xe5, 0xc8, 0x37, 0x2a, 0x6b, 0x00, 0x67, 0x6d, 0x51, 0x64, 0x40, 0xcc, 0x2e, 0xd1, 0xae, 0x4e, 0x12, 0x32, 0x07, 0x10, 0xcd, 0xe5, 0x37, 0x55, 0x18, 0x82, 0xb6, 0x9a, 0x37, 0xe8, 0x92, 0x3f, 0x04, 0x6c, 0x7c, 0x98, 0xa7, 0xfe, 0x68, 0x29, 0xb4, 0xcd, 0xba, 0x5d, 0xfb, 0x5d, 0x0e, 0x2d, 0x3e, 0xda, 0x44, 0x38, 0xf2, 0x0c, 0x39, 0x57, 0x8b, 0x43, 0x5d, 0x43, 0xd3, 0x25, 0x63, 0xa8, 0xbf, 0x67, 0x4b, 0x53, 0x5f, 0x95, 0x46, 0x78, 0xdd, 0x43, 0xde, 0x60, 0x57, 0xd3, 0x48, 0x5c, 0x23, 0x47, 0x01, 0x9b, 0x12, 0x54, 0x46, 0xf8, 0x00, 0xa6, 0xb5, 0xae, 0x8a, 0xd6, 0x7d, 0x81, 0x15, 0x9c, 0x9c, 0x55, 0xc8, 0x36, 0x28, 0xc7, 0x83, 0x23, 0xfa, 0x32, 0x3e, 0x32, 0xd4, 0x55, 0x3d, 0x39, 0xfc, 0x87, 0x26, 0x5e, 0x83, 0x77, 0xde, 0xcd, 0x14, 0xef, 0xd0, 0xdb, 0x9e, 0x91, 0x8f, 0x2f, 0x1e, 0xd0, 0x72, 0x5a, 0x6a, 0xe1, 0x39, 0x66, 0x57, 0xf3, 0x04, 0x9e, 0xf3, 0x82, 0x38, 0x5f, 0x10, 0xa6, 0x91, 0xd0, 0xe3, 0xf9, 0xaa, 0x2f, 0x1e, 0xce, 0x6f, 0x12, 0xb5, 0x0b, 0x2b, 0x4d, 0x02, 0x4e, 0x43, 0x67, 0x24, 0x09, 0xe6, 0x5a, 0xb0, 0x78, 0xa9, 0x9b, 0xac, 0xc9, 0x59, 0x4e, 0x8e, 0xe1, 0xb0, 0x55, 0x0b, 0xbe, 0xdd, 0x29, 0xc2 };
    static word earr[] = { 0x01, 0x00, 0x01 };
    static word darr[] = { 0x11, 0xe6, 0xfc, 0x07, 0x75, 0x99, 0xbd, 0xab, 0x49, 0xd5, 0x74, 0x57, 0x46, 0xc1, 0xde, 0xec, 0xae, 0x2c, 0x48, 0x79, 0xfc, 0x5e, 0x5d, 0x34, 0x43, 0x4a, 0x25, 0xca, 0xe1, 0xdf, 0x2d, 0x37, 0xa0, 0x24, 0x5f, 0x5c, 0xaa, 0x0c, 0xb4, 0x6c, 0xf7, 0xfe, 0x39, 0x20, 0xcf, 0x11, 0x9e, 0x3e, 0x8b, 0x66, 0x52, 0x9f, 0x80, 0xae, 0xfd, 0xca, 0x5e, 0x2a, 0x88, 0x86, 0x59, 0xa3, 0x55, 0xe7, 0xc0, 0xad, 0x73, 0xa6, 0x19, 0x10, 0x67, 0xe5, 0xe8, 0xda, 0xd8, 0x07, 0x0b, 0x63, 0xda, 0x34, 0xc9, 0xeb, 0xaf, 0x1f, 0x9d, 0x7f, 0xfd, 0x54, 0x62, 0x49, 0xe0, 0xef, 0x76, 0x80, 0xda, 0x8d, 0xfe, 0x70, 0x9e, 0x9b, 0xea, 0x99, 0x85, 0x96, 0x2e, 0x1e, 0xae, 0x07, 0x60, 0x3c, 0x6e, 0xd1, 0x56, 0x23, 0xc6, 0x05, 0x2b, 0x72, 0x26, 0x14, 0x57, 0x2c, 0x69, 0x13, 0x4c, 0xfb, 0x5c, 0xfe, 0x11, 0x77, 0xe8, 0x52, 0x88, 0xfd, 0x0e, 0x32, 0x57, 0x9d, 0x76, 0xcc, 0x40, 0xfb, 0x9c, 0xfe, 0x17, 0x0f, 0x0f, 0x62, 0x75, 0x60, 0x53, 0xf0, 0x1c, 0x9c, 0x29, 0x00, 0x91, 0x56, 0x53, 0x86, 0x88, 0x3a, 0x78, 0x46, 0x25, 0xe0, 0xb5, 0x77, 0x63, 0x43, 0xed, 0x71, 0x4e, 0x2a, 0x3b, 0xb8, 0x79, 0x57, 0xe3, 0xb8, 0x1c, 0xbf, 0xa6, 0x12, 0x8b, 0xde, 0x4f, 0x9e, 0xce, 0x86, 0xad, 0x90, 0x29, 0xe5, 0x30, 0x47, 0xd7, 0x14, 0xf6, 0x8e, 0x0a, 0xf4, 0xee, 0x30, 0x06, 0x70, 0x8d, 0x1a, 0xfd, 0x19, 0x90, 0x29, 0xba, 0x98, 0xd4, 0x0f, 0x47, 0x94, 0x87, 0xe2, 0x39, 0x99, 0x10, 0xd6, 0x39, 0x61, 0x1b, 0x6b, 0x14, 0x75, 0x30, 0x15, 0x11, 0xc7, 0xfe, 0xf8, 0x57, 0xd4, 0x33, 0xae, 0x1a, 0x2d, 0x9f, 0xed, 0x06, 0xdb, 0xf9, 0xc1, 0x9c, 0xac, 0xe9, 0x08, 0x6b, 0x7b, 0x87, 0x2f, 0xca, 0x44, 0xa7, 0x70, 0x90, 0x2b, 0x40, 0x9f, 0x5c, 0x6c, 0x44, 0x17, 0x6b, 0xf9, 0x52, 0xce, 0xf0, 0x11, 0x5a, 0xaf, 0x25, 0x90, 0xcd, 0x23, 0xa5, 0xc8, 0x2b, 0x4e, 0x34, 0xd4, 0x7b, 0xd3, 0x03, 0x33, 0xa7, 0xfe, 0x0c, 0x71, 0xc5, 0xbe, 0x7d, 0xe3, 0x76, 0x6a, 0x1c, 0xf9, 0x94, 0xde, 0xd4, 0x68, 0xec, 0xd6, 0xac, 0xfb, 0x1f, 0x73, 0x03, 0x21, 0xfe, 0x2a, 0xdd, 0x93, 0xa4, 0x8b, 0x5c, 0x41, 0x2c, 0xf1, 0xef, 0x60, 0x30, 0x02, 0xa5, 0xb7, 0xe3, 0xed, 0x36, 0xde, 0xf8, 0x3e, 0xf0, 0xc5, 0x93, 0x30, 0x93, 0xd9, 0x0f, 0xc1, 0xbd, 0x6f, 0x1c, 0xc1, 0xdb, 0x7f, 0xe8, 0x68, 0x9c, 0x88, 0xf9, 0x1f, 0x0a, 0x20, 0xcb, 0xde, 0xf2, 0xb9, 0x3e, 0xf5, 0xa3, 0x16, 0x5e, 0xdc, 0xf0, 0xe3, 0x84, 0xec, 0x12, 0x2f, 0x55, 0xb0, 0x5a, 0x38, 0x0f, 0x4e, 0x25, 0xb6, 0xe3, 0x1d };
    static word phiarr[] = { 0x8c, 0x14, 0x87, 0x10, 0xdb, 0x2f, 0x8d, 0xe0, 0xe9, 0x32, 0x9d, 0xad, 0xa3, 0xda, 0xe7, 0xf9, 0x5a, 0x8e, 0xab, 0xfa, 0x3d, 0x47, 0x99, 0x7e, 0x3e, 0x2a, 0x0e, 0x8b, 0x5c, 0xac, 0xa1, 0xfd, 0x50, 0xb3, 0xb9, 0xdb, 0xdc, 0xab, 0x75, 0x9f, 0x9b, 0xab, 0xe0, 0xf5, 0x80, 0xb5, 0xde, 0x7c, 0x7b, 0x1d, 0xa4, 0x36, 0xb2, 0xeb, 0x15, 0x13, 0x4b, 0x8e, 0xe2, 0x04, 0x75, 0x84, 0x24, 0xc6, 0x1d, 0x9f, 0x13, 0x2a, 0xb2, 0xbc, 0x45, 0x59, 0xcc, 0x66, 0xfd, 0x6b, 0x13, 0x45, 0x6e, 0x28, 0x95, 0xb6, 0xf1, 0x92, 0xc6, 0x2e, 0x51, 0x04, 0xf0, 0x15, 0xa9, 0x64, 0x98, 0x50, 0xff, 0xec, 0xd8, 0x60, 0x8b, 0x90, 0xeb, 0x25, 0xec, 0x04, 0x91, 0x34, 0x21, 0x20, 0xc4, 0x64, 0xac, 0x1e, 0x96, 0x65, 0xfe, 0x2e, 0xc7, 0x32, 0xd2, 0x16, 0xe9, 0xa9, 0xa0, 0x8e, 0x2c, 0x17, 0xb8, 0x46, 0x43, 0xb4, 0x46, 0xc8, 0x36, 0xc9, 0x64, 0xf4, 0xd0, 0x8b, 0x0b, 0x4e, 0x8b, 0x20, 0x0b, 0x8f, 0x57, 0x30, 0x6c, 0x33, 0xd8, 0x57, 0x6e, 0xaf, 0x10, 0xa7, 0x93, 0x1e, 0x13, 0x85, 0xad, 0x01, 0x51, 0xbe, 0x94, 0x13, 0x23, 0x83, 0x47, 0x0a, 0x51, 0xb8, 0xfc, 0x6b, 0x15, 0x06, 0xb7, 0xea, 0xef, 0xef, 0x68, 0x8d, 0x61, 0x8c, 0xe7, 0x23, 0xe2, 0x09, 0xb6, 0x4f, 0x45, 0x92, 0x0b, 0x0f, 0x71, 0xc3, 0x17, 0x7d, 0x97, 0x6e, 0x40, 0xe5, 0x79, 0x53, 0x6d, 0x81, 0x70, 0xbf, 0x8a, 0xce, 0x10, 0x9f, 0x98, 0xe3, 0x43, 0xcf, 0xef, 0xe7, 0xc7, 0xef, 0x45, 0x70, 0xe3, 0x70, 0x6b, 0xd7, 0x64, 0x37, 0x6e, 0x52, 0x0a, 0xa4, 0xa9, 0x05, 0x16, 0x98, 0x57, 0xf4, 0x47, 0xcc, 0x02, 0x56, 0xae, 0x57, 0xeb, 0x9c, 0xd6, 0xab, 0x30, 0x74, 0x3f, 0x6e, 0x9c, 0xfa, 0x4c, 0xda, 0x92, 0x91, 0xce, 0xbe, 0xd0, 0x02, 0x9d, 0xca, 0x89, 0xae, 0x9e, 0xca, 0x25, 0x41, 0xca, 0x0b, 0x81, 0xa1, 0xa7, 0x6a, 0x23, 0xe6, 0xac, 0xaa, 0xa0, 0xa5, 0xfc, 0x58, 0xbb, 0x79, 0x01, 0xd0, 0x23, 0xbf, 0x8a, 0x2b, 0xbf, 0xce, 0x65, 0xf1, 0xb7, 0x76, 0x82, 0xfe, 0xc1, 0x96, 0x3c, 0x35, 0x5f, 0xc9, 0x3a, 0x7a, 0xf1, 0xc5, 0x49, 0x35, 0xb1, 0xf6, 0xeb, 0x18, 0x23, 0x9e, 0x41, 0x7e, 0xf8, 0x4e, 0x54, 0x22, 0x32, 0x5b, 0xc5, 0xb1, 0x9f, 0xd9, 0xde, 0x04, 0x33, 0x05, 0x10, 0xb8, 0xf0, 0xd1, 0xad, 0x10, 0xa1, 0xc3, 0x67, 0x03, 0x50, 0x93, 0xd4, 0x1f, 0x6c, 0x06, 0x79, 0x8d, 0xf1, 0x80, 0xf4, 0xb3, 0x4a, 0x27, 0x56, 0x1c, 0x97, 0xfb, 0x6a, 0x4d, 0x46, 0x48, 0xcc, 0xa2, 0x75, 0x3c, 0x6c, 0x2c, 0xab, 0x6d, 0x46, 0x53, 0x4a, 0x63, 0x1f, 0x4d, 0xbf, 0x07, 0xa4, 0xb9, 0xc4, 0x72 };
#elif DTYPE == 32
    static word parr[] = { 0x4eab7f13, 0xc142bdff, 0x82879034, 0x5e3c0d4a, 0xd9ad3d19, 0xd68157ac, 0x0937b5a2, 0x24f39213, 0xe794adb7, 0x75318e12, 0x84f91942, 0x00db54a7, 0xcfd7e63f, 0x51259c9b, 0x24c3ad9d, 0xaddd6782, 0xf4958fd8, 0x12abed27, 0x64eb79c9, 0x2f516501, 0x87bed81a, 0x5b284154, 0x0b888460, 0x6703a631, 0xe794151d, 0x8475272f, 0xb47c403d, 0xa66289c1, 0xf5f5354c, 0xea8a6999, 0xe8609a7b, 0x499ab4ac, 0xa0bc98a6, 0xe1ef2117, 0x99b73765, 0x14a5a325, 0x51e7e7d9, 0xa2418902, 0xcccd5e58, 0xe3340fad, 0x208603bf, 0x469849ef, 0xf36655de, 0x3c4ca05c, 0xf206fdd1, 0x9ce1fe47, 0x4a7a31ed, 0x9751c933 };
    static word qarr[] = { 0x3db6d097, 0x37c8e582, 0x67006b2a, 0x4064516d, 0xaed12ecc, 0x0732124e, 0x37e5cd10, 0xb6821855, 0x92e8379a, 0x7c6c043f, 0x68fea798, 0xbacdb429, 0x0e5dfb5d, 0x44da3e2d, 0x390cf238, 0x5d438b57, 0x6325d343, 0x4b67bfa8, 0x46955f53, 0xde43dd78, 0x48d35760, 0x0147235c, 0x4654129b, 0xb5a600f8, 0x7dd68aae, 0x9c9c1581, 0x2836c855, 0xfa2383c7, 0xd4323e32, 0xfc393d55, 0x835e2687, 0x14cdde77, 0x9edbd0ef, 0x1e2f8f91, 0x6a5a72d0, 0x576639e1, 0xf39e04f3, 0x105f3882, 0xe3d091a6, 0x1e2faaf9, 0xb5126fce, 0x024d2b0b, 0x2467434e, 0xb05ae609, 0xac9ba978, 0x8e4e59c9, 0x0b55b0e1, 0xc229ddbe };
    static word earr[] = { 0x00010001 };
    static word darr[] = { 0x07fce611, 0xabbd9975, 0x5774d549, 0xecdec146, 0x79482cae, 0x345d5efc, 0xca254a43, 0x372ddfe1, 0x5c5f24a0, 0x6cb40caa, 0x2039fef7, 0x3e9e11cf, 0x9f52668b, 0xcafdae80, 0x86882a5e, 0xe755a359, 0xa673adc0, 0xe5671019, 0x07d8dae8, 0x34da630b, 0x1fafebc9, 0x54fd7f9d, 0xefe04962, 0x8dda8076, 0x9b9e70fe, 0x968599ea, 0x07ae1e2e, 0xd16e3c60, 0x05c62356, 0x1426722b, 0x13692c57, 0xfe5cfb4c, 0x52e87711, 0x320efd88, 0xcc769d57, 0xfe9cfb40, 0x620f0f17, 0xf0536075, 0x00299c1c, 0x86535691, 0x46783a88, 0x77b5e025, 0x71ed4363, 0xb83b2a4e, 0xb8e35779, 0x12a6bf1c, 0x9e4fde8b, 0x90ad86ce, 0x4730e529, 0x8ef614d7, 0x30eef40a, 0x1a8d7006, 0x299019fd, 0x0fd498ba, 0xe2879447, 0xd6109939, 0x6b1b6139, 0x15307514, 0xf8fec711, 0xae33d457, 0xed9f2d1a, 0xc1f9db06, 0x08e9ac9c, 0x2f877b6b, 0x70a744ca, 0x9f402b90, 0x17446c5c, 0xce52f96b, 0xaf5a11f0, 0x23cd9025, 0x4e2bc8a5, 0xd37bd434, 0xfea73303, 0xbec5710c, 0x6a76e37d, 0xde94f91c, 0xd6ec68d4, 0x731ffbac, 0x2afe2103, 0x8ba493dd, 0xf12c415c, 0x023060ef, 0xede3b7a5, 0x3ef8de36, 0x3093c5f0, 0xc10fd993, 0xc11c6fbd, 0x68e87fdb, 0x1ff9889c, 0xdecb200a, 0xf53eb9f2, 0xdc5e16a3, 0xec84e3f0, 0xb0552f12, 0x4e0f385a, 0x1de3b625 };
    static word phiarr[] = { 0x1087148c, 0xe08d2fdb, 0xad9d32e9, 0xf9e7daa3, 0xfaab8e5a, 0x7e99473d, 0x8b0e2a3e, 0xfda1ac5c, 0xdbb9b350, 0x9f75abdc, 0xf5e0ab9b, 0x7cdeb580, 0x36a41d7b, 0x1315ebb2, 0x04e28e4b, 0xc6248475, 0x2a139f1d, 0x5945bcb2, 0x6bfd66cc, 0x286e4513, 0x92f1b695, 0x04512ec6, 0x64a915f0, 0xecff5098, 0x908b60d8, 0x04ec25eb, 0x20213491, 0x1eac64c4, 0x2efe6596, 0x16d232c7, 0x8ea0a9e9, 0x46b8172c, 0xc846b443, 0xf464c936, 0x4e0b8bd0, 0x8f0b208b, 0x336c3057, 0xaf6e57d8, 0x1e93a710, 0x01ad8513, 0x1394be51, 0x0a478323, 0x6bfcb851, 0xeab70615, 0x8d68efef, 0x23e78c61, 0x4fb609e2, 0x0f0b9245, 0x7d17c371, 0xe5406e97, 0x816d5379, 0xce8abf70, 0xe3989f10, 0xe7efcf43, 0x7045efc7, 0xd76b70e3, 0x526e3764, 0x05a9a40a, 0xf4579816, 0x5602cc47, 0x9ceb57ae, 0x7430abd6, 0xfa9c6e3f, 0x9192da4c, 0x02d0bece, 0xae89ca9d, 0x4125ca9e, 0xa1810bca, 0xe6236aa7, 0xa5a0aaac, 0x79bb58fc, 0xbf23d001, 0xcebf2b8a, 0x76b7f165, 0x96c1fe82, 0xc95f353c, 0xc5f17a3a, 0xf6b13549, 0x9e2318eb, 0x4ef87e41, 0x5b322254, 0xd99fb1c5, 0x053304de, 0xd1f0b810, 0xc3a110ad, 0x93500367, 0x066c1fd4, 0x80f18d79, 0x274ab3f4, 0xfb971c56, 0x48464d6a, 0x3c75a2cc, 0x6dab2c6c, 0x634a5346, 0x07bf4d1f, 0x72c4b9a4 };

#elif DTYPE==64
    static word parr[] = { 0xc142bdff4eab7f13, 0x5e3c0d4a82879034, 0xd68157acd9ad3d19, 0x24f392130937b5a2, 0x75318e12e794adb7, 0x00db54a784f91942, 0x51259c9bcfd7e63f, 0xaddd678224c3ad9d, 0x12abed27f4958fd8, 0x2f51650164eb79c9, 0x5b28415487bed81a, 0x6703a6310b888460, 0x8475272fe794151d, 0xa66289c1b47c403d, 0xea8a6999f5f5354c, 0x499ab4ace8609a7b, 0xe1ef2117a0bc98a6, 0x14a5a32599b73765, 0xa241890251e7e7d9, 0xe3340fadcccd5e58, 0x469849ef208603bf, 0x3c4ca05cf36655de, 0x9ce1fe47f206fdd1, 0x9751c9334a7a31ed };
    static word qarr[] = { 0x37c8e5823db6d097, 0x4064516d67006b2a, 0x0732124eaed12ecc, 0xb682185537e5cd10, 0x7c6c043f92e8379a, 0xbacdb42968fea798, 0x44da3e2d0e5dfb5d, 0x5d438b57390cf238, 0x4b67bfa86325d343, 0xde43dd7846955f53, 0x0147235c48d35760, 0xb5a600f84654129b, 0x9c9c15817dd68aae, 0xfa2383c72836c855, 0xfc393d55d4323e32, 0x14cdde77835e2687, 0x1e2f8f919edbd0ef, 0x576639e16a5a72d0, 0x105f3882f39e04f3, 0x1e2faaf9e3d091a6, 0x024d2b0bb5126fce, 0xb05ae6092467434e, 0x8e4e59c9ac9ba978, 0xc229ddbe0b55b0e1 };
    static word earr[] = { 0x0000000000010001 };
    static word darr[] = { 0xabbd997507fce611, 0xecdec1465774d549, 0x345d5efc79482cae, 0x372ddfe1ca254a43, 0x6cb40caa5c5f24a0, 0x3e9e11cf2039fef7, 0xcafdae809f52668b, 0xe755a35986882a5e, 0xe5671019a673adc0, 0x34da630b07d8dae8, 0x54fd7f9d1fafebc9, 0x8dda8076efe04962, 0x968599ea9b9e70fe, 0xd16e3c6007ae1e2e, 0x1426722b05c62356, 0xfe5cfb4c13692c57, 0x320efd8852e87711, 0xfe9cfb40cc769d57, 0xf0536075620f0f17, 0x8653569100299c1c, 0x77b5e02546783a88, 0xb83b2a4e71ed4363, 0x12a6bf1cb8e35779, 0x90ad86ce9e4fde8b, 0x8ef614d74730e529, 0x1a8d700630eef40a, 0x0fd498ba299019fd, 0xd6109939e2879447, 0x153075146b1b6139, 0xae33d457f8fec711, 0xc1f9db06ed9f2d1a, 0x2f877b6b08e9ac9c, 0x9f402b9070a744ca, 0xce52f96b17446c5c, 0x23cd9025af5a11f0, 0xd37bd4344e2bc8a5, 0xbec5710cfea73303, 0xde94f91c6a76e37d, 0x731ffbacd6ec68d4, 0x8ba493dd2afe2103, 0x023060eff12c415c, 0x3ef8de36ede3b7a5, 0xc10fd9933093c5f0, 0x68e87fdbc11c6fbd, 0xdecb200a1ff9889c, 0xdc5e16a3f53eb9f2, 0xb0552f12ec84e3f0, 0x1de3b6254e0f385a };
    static word phiarr[] = { 0xe08d2fdb1087148c, 0xf9e7daa3ad9d32e9, 0x7e99473dfaab8e5a, 0xfda1ac5c8b0e2a3e, 0x9f75abdcdbb9b350, 0x7cdeb580f5e0ab9b, 0x1315ebb236a41d7b, 0xc624847504e28e4b, 0x5945bcb22a139f1d, 0x286e45136bfd66cc, 0x04512ec692f1b695, 0xecff509864a915f0, 0x04ec25eb908b60d8, 0x1eac64c420213491, 0x16d232c72efe6596, 0x46b8172c8ea0a9e9, 0xf464c936c846b443, 0x8f0b208b4e0b8bd0, 0xaf6e57d8336c3057, 0x01ad85131e93a710, 0x0a4783231394be51, 0xeab706156bfcb851, 0x23e78c618d68efef, 0x0f0b92454fb609e2, 0xe5406e977d17c371, 0xce8abf70816d5379, 0xe7efcf43e3989f10, 0xd76b70e37045efc7, 0x05a9a40a526e3764, 0x5602cc47f4579816, 0x7430abd69ceb57ae, 0x9192da4cfa9c6e3f, 0xae89ca9d02d0bece, 0xa1810bca4125ca9e, 0xa5a0aaace6236aa7, 0xbf23d00179bb58fc, 0x76b7f165cebf2b8a, 0xc95f353c96c1fe82, 0xf6b13549c5f17a3a, 0x4ef87e419e2318eb, 0xd99fb1c55b322254, 0xd1f0b810053304de, 0x93500367c3a110ad, 0x80f18d79066c1fd4, 0xfb971c56274ab3f4, 0x3c75a2cc48464d6a, 0x634a53466dab2c6c, 0x72c4b9a407bf4d1f };
#endif

    bi_set_by_array(p, NONNEGATIVE, parr, plen);
    bi_set_by_array(q, NONNEGATIVE, qarr, qlen);
    bi_set_by_array(phi, NONNEGATIVE, phiarr, philen);
    bi_set_by_array(e, NONNEGATIVE, earr, elen);
    bi_set_by_array(d, NONNEGATIVE, darr, dlen);
    bi_mul_textbook(n, *p, *q);
    if (*p == NULL || *q == NULL || *phi == NULL || *e == NULL || *d == NULL || *n == NULL) {
        return;
    }
}

/**
 * @brief Encrypts a plaintext using RSA.
 * @param ct Pointer to the variable to store the ciphertext.
 * @param pt Pointer to the plaintext.
 * @return 0 if successful, -1 on failure.
 */
int RSA_encrypt(bigint** ct, bigint* pt) {
    bigint* p = NULL;
    bigint* q = NULL;
    bigint* phi = NULL;
    bigint* e = NULL;
    bigint* d = NULL;
    bigint* n = NULL;



    bi_new(&p, plen);
    bi_new(&q, qlen);
    bi_new(&phi, philen);
    bi_new(&e, elen);
    bi_new(&d, dlen);
    bi_new(&n, nlen);

    // set RSA parameter
    get_RSA_param(&p, &q, &phi, &e, &d, &n);
    bi_LtR_mod(ct, &pt, e, n);

    bi_delete(&p);
    bi_delete(&q);
    bi_delete(&phi);
    bi_delete(&e);
    bi_delete(&d);
    bi_delete(&n);

    return 0;
}

/**
 * @brief Decrypts a ciphertext using RSA.
 * @param pt Pointer to the variable to store the decrypted plaintext.
 * @param ct Pointer to the ciphertext.
 * @return 0 if successful, -1 on failure.
 */
int RSA_decrypt(bigint** pt, bigint* ct) {
    bigint* p = NULL;
    bigint* q = NULL;
    bigint* phi = NULL;
    bigint* e = NULL;
    bigint* d = NULL;
    bigint* n = NULL;

    bi_new(&p, plen);
    bi_new(&q, qlen);
    bi_new(&phi, philen);
    bi_new(&e, elen);
    bi_new(&d, dlen);
    bi_new(&n, nlen);

    // set RSA parameter
    get_RSA_param(&p, &q, &phi, &e, &d, &n);
    bi_LtR_mod(pt, &ct, d, n);

    bi_delete(&p);
    bi_delete(&q);
    bi_delete(&phi);
    bi_delete(&e);
    bi_delete(&d);
    bi_delete(&n);

    return 0;
}

/**
 * @brief Verifies the result of RSA decryption.
 * Compares the original plaintext with the decrypted plaintext to ensure correctness.
 * @param m Pointer to the original plaintext.
 * @param pt Pointer to the decrypted plaintext.
 * @return 0 if verification is successful, -1 if verification fails.
 */
int RSA_verify(bigint* m, bigint* pt) {
    if (m->wordlen != pt->wordlen) {
        return -1;
    }

    for (int i = 0; i < m->wordlen; i++) {
        if (m->a[i] != pt->a[i]) {
            fprintf(stderr, "RSA verify failed\n");
            return -1;
        }
    }

    return 0;
}

