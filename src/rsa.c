#include <stdio.h>
#include "rsa.h"

//! Caution: prime p and q are hard-coded

void get_RSA_param(bigint** p, bigint** q, bigint** phi, bigint** e, bigint** d, bigint** n){
    
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
    bi_new(&phi, llen);
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
    bi_new(&phi, llen);
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

