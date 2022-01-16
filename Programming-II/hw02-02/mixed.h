#ifndef MIXEDNUM
#define MIXEDNUM
#include <stdint.h>
#include "util.h"
typedef struct _sMixedNumber
{
    /* data */
    int64_t whole;
    int64_t deno;
    int64_t frac;
} sMixedNumber;

int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c);
int mixed_print( const sMixedNumber number);
void mixed_add( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
void mixed_sub( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
void mixed_mul( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
void mixed_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
#endif