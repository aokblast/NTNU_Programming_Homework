#include <stdlib.h>
#include "mixed.h"
#include <stdio.h>
#include <stdbool.h>

bool isInt(const sMixedNumber *r){
    if(r->deno == 0 && r->frac == 0) return 1;
    return 0;
}

sMixedNumber tofake(const sMixedNumber * r) {
    if(isInt(r)) {
        sMixedNumber result = {0, r->whole, 1};
        return result;
    }
    sMixedNumber result = {0, r->whole * r->frac + r->deno, r->frac};
    return result;
}

void justify(sMixedNumber *r) {
    int64_t divisor = gcd(abs(r->frac), abs(r->deno));
    r->deno /= divisor;
    r->frac /= divisor;
}

void carry(sMixedNumber *r) {
    if(r->deno < 0 && r->whole != 0) {
        --(r->whole);
        r->deno = abs(r->deno);
        r->deno = r->frac - r->deno;
    }    
}

int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c){
    if(b > c || (c == 0 && b!=0) || (c != 0 && b == 0) || gcd(abs(b), c) != 1 ) return 0;
    pNumber->whole = a;
    pNumber->deno = b;
    pNumber->frac = c;
    return 1;
}

int mixed_print( const sMixedNumber number) {
    return printf("(%ld, %ld, %ld)\n", number.whole, number.deno, number.frac);
}

void mixed_add( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.frac + f2.deno * f1.frac;
    pNumber->frac = f1.frac * f2.frac;
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->deno == 0) pNumber->frac = 0; 
    justify(pNumber);
    carry(pNumber);
}

void mixed_sub( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.frac - f2.deno * f1.frac;
    pNumber->frac = f1.frac * f2.frac;
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
    carry(pNumber);
}

void mixed_mul( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.deno;
    pNumber->frac = f1.frac * f2.frac; 
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
    carry(pNumber);
}

void mixed_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.frac;
    pNumber->frac = f1.frac * f2.deno;    
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
    carry(pNumber);
}