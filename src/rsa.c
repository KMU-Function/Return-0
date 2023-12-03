#include <stdio.h>
#include "rsa.h"

//! Caution: prime p and q are hard-coded

#if DTYPE==8
#define plen    64
#define qlen    64
#define elen    4
#define dlen    128
#define nlen    128
#define philen  128
#elif DTYPE == 32
#define plen    16
#define qlen    16
#define elen    1
#define dlen    32
#define nlen    32
#define philen  32
#elif DTYPE==64
#define plen    8
#define qlen    8
#define elen    1
#define dlen    16
#define nlen    16
#define philen  16
#endif

void get_RSA_param(bigint** p, bigint** q, bigint** phi, bigint** e, bigint** d, bigint** n){
#if DTYPE==8
    static word parr[] =  {0x65, 0xb5, 0x4b, 0x61, 0x97, 0x75, 0xbf, 0xe1, 0x03, 0x85, 0xe3, 0x62, 0x6c, 0x41, 0x6a, 0xed, 0xb8, 0x09, 0xe2, 0xcb, 0x07, 0xc3, 0x92, 0x6f, 0x50, 0x0d, 0xa3, 0xa5, 0x2f, 0xf5, 0x06, 0x37, 0x05, 0x0c, 0x4b, 0xa6, 0x8c, 0x59, 0xd9, 0xc7, 0x68, 0x24, 0xad, 0xe3, 0x8b, 0x2e, 0x50, 0xf6, 0xea, 0x67, 0x9a, 0x00, 0x82, 0x91, 0x10, 0x5f, 0xfc, 0xd8, 0xef, 0x0d, 0xd8, 0xae, 0x14, 0xd5};
    static word qarr[] =  {0x45, 0x4e, 0x45, 0xee, 0xc4, 0xe7, 0x80, 0x21, 0x38, 0x94, 0xd3, 0x36, 0x91, 0xc2, 0x94, 0x0a, 0x4e, 0x46, 0x27, 0x82, 0x1a, 0xb6, 0x2f, 0xf2, 0x24, 0x59, 0xa3, 0x18, 0x7f, 0xa8, 0x1c, 0xfe, 0xec, 0x4d, 0xc6, 0x53, 0xc9, 0x0e, 0x52, 0xc2, 0x3b, 0x6c, 0x9c, 0xcd, 0x1d, 0xf7, 0xd8, 0x60, 0x79, 0x3a, 0x18, 0x63, 0x73, 0x6d, 0x87, 0x73, 0xd2, 0xd1, 0xcb, 0x58, 0xbe, 0xbf, 0x5c, 0x96};
    static word earr[] =  {0x00, 0x01, 0x00, 0x01};
    static word darr[] =  {0xc1, 0xf4, 0x99, 0xe6, 0x64, 0x5b, 0x3f, 0x92, 0x00, 0x77, 0x55, 0x9a, 0x39, 0x5c, 0x83, 0xd0, 0xed, 0x78, 0x2d, 0x80, 0x81, 0x40, 0x40, 0x0a, 0x3c, 0x12, 0xa0, 0x17, 0x67, 0x66, 0x93, 0x3c, 0x53, 0x3b, 0x2c, 0x2a, 0x3f, 0x92, 0x60, 0x1e, 0x78, 0xc8, 0xee, 0xc5, 0xfa, 0x37, 0xc8, 0xfa, 0xc8, 0xeb, 0x82, 0x75, 0x3f, 0x7d, 0xee, 0xd7, 0xef, 0x07, 0x41, 0x20, 0x65, 0x7c, 0xed, 0x14, 0xd4, 0x48, 0xbb, 0x31, 0x1b, 0x60, 0x31, 0x7a, 0xdb, 0x20, 0x50, 0x32, 0xf6, 0x7d, 0x5c, 0xdf, 0x2a, 0xb7, 0x5a, 0x61, 0xa7, 0xfb, 0x08, 0x72, 0x5b, 0x5d, 0x3f, 0x73, 0xaf, 0x62, 0x65, 0x04, 0x73, 0x71, 0x5e, 0xae, 0xf2, 0x58, 0x85, 0x8c, 0xd4, 0x08, 0x99, 0xb1, 0xea, 0xae, 0x9a, 0x9b, 0x19, 0x30, 0x81, 0xf7, 0x7b, 0x28, 0x6e, 0x6c, 0xbf, 0xe1, 0xa7, 0x80, 0x4d, 0x18, 0x12, 0x48};
    static word phiarr[] =  {0x90, 0xa6, 0x54, 0xc5, 0x75, 0x14, 0x95, 0x1f, 0x8c, 0xe2, 0xa8, 0x9c, 0x22, 0x66, 0xe1, 0x08, 0xcf, 0x4c, 0x28, 0x23, 0x14, 0xbe, 0xe9, 0x04, 0x5e, 0xda, 0x45, 0xf4, 0x35, 0x01, 0x56, 0x05, 0x2f, 0x1b, 0x42, 0x8f, 0xe1, 0x42, 0xc5, 0x32, 0x65, 0xf8, 0xb5, 0xe3, 0xb6, 0xb1, 0xc1, 0x5e, 0x00, 0x84, 0x96, 0xd7, 0x25, 0x3e, 0xdb, 0x9f, 0x1c, 0x1b, 0xff, 0x70, 0x3f, 0x1f, 0x0f, 0xd5, 0x8c, 0x03, 0xb3, 0x5b, 0x66, 0xb8, 0x45, 0x97, 0xb4, 0xe5, 0xfb, 0x43, 0x58, 0x15, 0x66, 0xff, 0xa7, 0x82, 0x96, 0x01, 0xf2, 0xc0, 0x7f, 0x55, 0xbe, 0xf7, 0x34, 0x7c, 0xee, 0xa0, 0xee, 0xed, 0x2f, 0x95, 0x8e, 0x93, 0xa2, 0xee, 0xf9, 0xda, 0x70, 0x52, 0x47, 0x1e, 0xdf, 0x7b, 0x24, 0x16, 0x47, 0x95, 0x0b, 0x8d, 0x63, 0x89, 0x49, 0x1d, 0x3c, 0x44, 0x85, 0x4b, 0x7a, 0x51, 0x27, 0x7d};
#elif DTYPE == 32
    word parr[] =  {0x614bb565, 0xe1bf7597, 0x62e38503, 0xed6a416c, 0xcbe209b8, 0x6f92c307, 0xa5a30d50, 0x3706f52f, 0xa64b0c05, 0xc7d9598c, 0xe3ad2468, 0xf6502e8b, 0x009a67ea, 0x5f109182, 0x0defd8fc, 0xd514aed8};
    word qarr[] =  {0xee454e45, 0x2180e7c4, 0x36d39438, 0x0a94c291, 0x8227464e, 0xf22fb61a, 0x18a35924, 0xfe1ca87f, 0x53c64dec, 0xc2520ec9, 0xcd9c6c3b, 0x60d8f71d, 0x63183a79, 0x73876d73, 0x58cbd1d2, 0x965cbfbe};   
    word earr[] =  {0x00010001};
    word darr[] =  {0xe699f4c1, 0x923f5b64, 0x9a557700, 0xd0835c39, 0x802d78ed, 0x0a404081, 0x17a0123c, 0x3c936667, 0x2a2c3b53, 0x1e60923f, 0xc5eec878, 0xfac837fa, 0x7582ebc8, 0xd7ee7d3f, 0x204107ef, 0x14ed7c65, 0x31bb48d4, 0x7a31601b, 0x325020db, 0xdf5c7df6, 0x615ab72a, 0x7208fba7, 0x733f5d5b, 0x046562af, 0xae5e7173, 0x8c8558f2, 0xb19908d4, 0x9b9aaeea, 0xf7813019, 0x6c6e287b, 0x80a7e1bf, 0x4812184d};
    word phiarr[] =  {0xc554a690, 0x1f951475, 0x9ca8e28c, 0x08e16622, 0x23284ccf, 0x04e9be14, 0xf445da5e, 0x05560135, 0x8f421b2f, 0x32c542e1, 0xe3b5f865, 0x5ec1b1b6, 0xd7968400, 0x9fdb3e25, 0x70ff1b1c, 0xd50f1f3f, 0x5bb3038c, 0x9745b866, 0x43fbe5b4, 0xff661558, 0x019682a7, 0x557fc0f2, 0x7c34f7be, 0xedeea0ee, 0x938e952f, 0xdaf9eea2, 0x1e475270, 0x16247bdf, 0x8d0b9547, 0x1d498963, 0x4b85443c, 0x7d27517a};
#elif DTYPE==64
    static word parr[] =  {0xe1bf7597614bb565, 0xed6a416c62e38503, 0x6f92c307cbe209b8, 0x3706f52fa5a30d50, 0xc7d9598ca64b0c05, 0xf6502e8be3ad2468, 0x5f109182009a67ea, 0xd514aed80defd8fc};
    static word qarr[] =  {0x2180e7c4ee454e45, 0x0a94c29136d39438, 0xf22fb61a8227464e, 0xfe1ca87f18a35924, 0xc2520ec953c64dec, 0x60d8f71dcd9c6c3b, 0x73876d7363183a79, 0x965cbfbe58cbd1d2};
    static word earr[] =  {0x0000000000010001};
    static word darr[] =  {0x923f5b64e699f4c1, 0xd0835c399a557700, 0x0a404081802d78ed, 0x3c93666717a0123c, 0x1e60923f2a2c3b53, 0xfac837fac5eec878, 0xd7ee7d3f7582ebc8, 0x14ed7c65204107ef, 0x7a31601b31bb48d4, 0xdf5c7df6325020db, 0x7208fba7615ab72a, 0x046562af733f5d5b, 0x8c8558f2ae5e7173, 0x9b9aaeeab19908d4, 0x6c6e287bf7813019, 0x4812184d80a7e1bf};
    static word phiarr[] =  {0x1f951475c554a690, 0x08e166229ca8e28c, 0x04e9be1423284ccf, 0x05560135f445da5e, 0x32c542e18f421b2f, 0x5ec1b1b6e3b5f865, 0x9fdb3e25d7968400, 0xd50f1f3f70ff1b1c, 0x9745b8665bb3038c, 0xff66155843fbe5b4, 0x557fc0f2019682a7, 0xedeea0ee7c34f7be, 0xdaf9eea2938e952f, 0x16247bdf1e475270, 0x1d4989638d0b9547, 0x7d27517a4b85443c};
#endif

    bi_set_by_array(p, NONNEGATIVE, parr, plen);
    bi_set_by_array(q, NONNEGATIVE, qarr, qlen);
    bi_set_by_array(phi, NONNEGATIVE, phiarr, philen);
    bi_set_by_array(e, NONNEGATIVE, earr, elen);
    bi_set_by_array(d, NONNEGATIVE, darr, dlen);
    bi_mul_textbook(n, *p, *q);
    if(*p == NULL || *q == NULL || *phi == NULL || *e == NULL || *d == NULL || *n == NULL){
        return;
    }
}

int RSA_encrypt(bigint** ct, bigint* pt){
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

int RSA_decrypt(bigint** pt, bigint* ct){
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
    bi_show_hex_inorder(p);
    bi_LtR_mod(pt, &ct, d, n);

    bi_delete(&p);
    bi_delete(&q);
    bi_delete(&phi);
    bi_delete(&e);
    bi_delete(&d);
    bi_delete(&n);

    return 0;
}

int RSA_verify(bigint* m, bigint* pt){
    if(m->wordlen != pt->wordlen){
        return -1;
    }
    
    for(int i = 0; i < m->wordlen; i++){
        if(m->a[i] != pt->a[i]){
            fprintf(stderr, "RSA verify failed\n");
            return -1;
        }
    }

    return 0;
}

