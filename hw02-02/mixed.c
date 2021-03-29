#include <stdlib.h>
#include "mixed.h"
#include <stdio.h>
#include <stdbool.h>

bool isInt(const sMixedNumber *r){
    if(r->deno == 0 && r->frac == 0) return true;
    return false;
}

sMixedNumber tofake(const sMixedNumber * r) {
    if(isInt(r)) {
        sMixedNumber result = {0, r->whole, 1};
        return result;
    }
    int pos = 1;
    if(r->whole < 0 || r->deno < 0) pos = -1;
    sMixedNumber result = {0, pos * (abs(r->whole * r->frac) + abs(r->deno)), r->frac};
    return result;
}

void justify(sMixedNumber *r) {
    int64_t divisor = gcd(abs(r->frac), abs(r->deno));
    r->deno /= divisor;
    r->frac /= divisor;
}

int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c){
    if ((c == 0) ^ (b == 0)) return -1;
    else if(a != 0 && ((b < 0) || (c < 0))) return -1;
    else if(a == 0 && c < 0) return -1;
    pNumber->whole = a;
    pNumber->deno = b;
    pNumber->frac = c;
    return 0;
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
    if(pNumber->whole) pNumber->deno = abs(pNumber->deno);
    if(pNumber->deno == 0) pNumber->frac = 0; 
    justify(pNumber);
}

void mixed_sub( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.frac - f2.deno * f1.frac;
    pNumber->frac = f1.frac * f2.frac;
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->whole) pNumber->deno = abs(pNumber->deno);
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
}

void mixed_mul( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.deno;
    pNumber->frac = f1.frac * f2.frac; 
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->whole) pNumber->deno = abs(pNumber->deno);
    else {pNumber->deno *= (pNumber->frac / abs(pNumber->frac)), pNumber->frac = abs(pNumber->frac);}
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
}

void mixed_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2) {
    if(r2.deno == 0 && r2.frac == 0 && r2.whole == 0) {
        pNumber->whole = pNumber->deno = pNumber->frac = 0;
        printf("Wrong divisor.\n");
        return;
    }
    sMixedNumber f1 = tofake(&r1);
    sMixedNumber f2 = tofake(&r2);
    pNumber->deno = f1.deno * f2.frac;
    pNumber->frac = f1.frac * f2.deno;    
    pNumber->whole = pNumber->deno / pNumber->frac;
    pNumber->deno %= pNumber->frac;
    if(pNumber->whole) pNumber->deno = abs(pNumber->deno);
    else {pNumber->deno *= (pNumber->frac / abs(pNumber->frac)), pNumber->frac = abs(pNumber->frac);}
    if(pNumber->deno == 0) pNumber->frac = 0;
    justify(pNumber);
}