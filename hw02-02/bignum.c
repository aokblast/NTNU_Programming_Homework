#include "bignum.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void print( const sBigNum num ) {
    //printf("%lu", num.size);
    if(num.sign == -1) printf("-");
    for(int i = num.size - 1;i >= 0; --i) {
        printf("%d", num.num[i]);
    }
    printf("\n");
}

void tozero(sBigNum *num) {
    for(int i = MAXLEN - 1; i >= 0; --i) num->num[i] = 0;
}

void swapBigNum(sBigNum *num1, sBigNum *num2) {
    sBigNum tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

int32_t set( sBigNum *pNum, char *str ) {
    char *iter = str;
    if(*iter == '-') pNum->sign = -1, ++iter;
    else pNum->sign = 1;
    pNum->size = strlen(iter);
    for(size_t i = strlen(iter) - 1;*iter != '\0'; ++iter, --i) {
        if(!isdigit(*iter)) return 0;
        pNum->num[i] = (*iter - '0');
    }
    return 1;
}

int32_t compare( const sBigNum num01 , const sBigNum num02 ) {
    if(num01.sign == 0 && num02.sign == 1) return 1;
    else if(num01.sign == 1 && num02.sign == 0) return -1;
    else if(num01.size > num02.size) return num01.sign;
    else if(num02.size > num01.size) return -num01.sign;
    else {
        for(int i = num01.size - 1; i >= 0; --i) {
            if(num01.num[i] > num02.num[i]) return num01.sign;
            else if (num02.num[i] > num01.num[i]) return -num01.sign;
        }
    }
    return 0;
}

int32_t digits( const sBigNum num ){
    return num.size;
}

void add( sBigNum *pResult ,const sBigNum num01 , const sBigNum num02 ) {
    if(num01.sign == 1 && num02.sign == -1) {
        sBigNum *ptr = (sBigNum *)&num02;
        ptr->sign = 1;
        subtract(pResult, num01, num02);
        return;
    }
    else if(num01.sign == -1 && num02.sign == 1) {
        sBigNum *ptr = (sBigNum *)&num01;
        ptr->sign = 1;
        pResult->sign = -1;
        subtract(pResult, num02, num01);
        return;
    }
    tozero(pResult);
    int carry = 0;
    size_t max_size = (num01.size > num02.size ? num01.size : num02.size);
    for(size_t i = 0; i < max_size; ++i) {
        pResult->num[i] = carry;
        if(i < num01.size) pResult->num[i] += num01.num[i];
        if(i < num02.size) pResult->num[i] += num02.num[i];
        carry = pResult->num[i] / 10;
        pResult->num[i] %= 10; 
    }
    if(carry) pResult->num[max_size++] = 1;
    pResult->size = max_size;
    if(num01.sign == -1)pResult->sign = num01.sign;
}

void subtract( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 ) {
    if(num01.sign == 1 && num02.sign == -1) {
        sBigNum *ptr = (sBigNum *)&num02;
        ptr->sign = 1;
        add(pResult, num01, num02);
        return;
    }
    else if(num01.sign == -1 && num02.sign == 1) {
        sBigNum *ptr = (sBigNum *)&num01;
        ptr->sign = 1;
        pResult->sign = -1;
        add(pResult, num02, num01);
        return;
    }else if(num01.sign == -1 && num02.sign == -1) {
        sBigNum *ptr = (sBigNum *)&num01;
        ptr->sign = 1;
        ptr = (sBigNum *)&num02;
        ptr->sign = 1;
        subtract(pResult, num02, num01);
        return;
    }
    if(compare(num01, num02) == -1) pResult->sign = -1,swapBigNum((sBigNum *) &num01, (sBigNum *) &num02);
    tozero(pResult);
    size_t max_size = (num01.size > num02.size ? num01.size : num02.size);
    int carry = 0;
    for(size_t i = 0; i < max_size; ++i) {
        if(i < num01.size) pResult->num[i] += num01.num[i];
        if(i < num02.size) pResult->num[i] -= num02.num[i];
        pResult->num[i] -= carry;
        if(pResult->num[i] < 0) carry = 1, pResult->num[i] += 10;
        else carry = 0;
    }
    if(carry == 1 && pResult->num[max_size] < 0)  pResult->num[max_size++] = -1;
    while(max_size != 1 && pResult->num[max_size - 1] == 0) --max_size;
    if(pResult->num[0] == 0 && max_size == 1) pResult->size = 1, pResult->sign = 1;
    else pResult->size = max_size;
    if(pResult->num[max_size - 1] < 0) pResult->num[max_size - 1] = -pResult->num[max_size - 1], pResult->sign = -1;
    
}

void multiply( sBigNum *pResult , const sBigNum num01 , const sBigNum num02 ) {
    tozero(pResult);
    int carry = 0;
    pResult->sign = num02.sign * num01.sign;
    for(size_t i = 0; i < num01.size; ++i) {
        for(size_t j = 0; j < num02.size; ++j) {
            pResult->num[i + j] += num01.num[i] * num02.num[j];
            carry = pResult->num[i + j] / 10;
            pResult->num[i + j] %= 10;
            int k = 1;
            while(carry != 0) {
                pResult->num[i + j + k] = pResult->num[i + j + k] + carry;
                carry = pResult->num[i + j + k] / 10;
                pResult->num[i + j + k] %= 10;
                ++k;
            }
        }
    }
    pResult->size = num01.size + num02.size - 1;
    if(pResult->num[pResult->size] != 0) ++pResult->size;
    while(pResult->size != 1 && pResult->num[pResult->size - 1] == 0) --pResult->size;
    if(pResult->num[0] == 0 && pResult->size == 1) pResult->size = 1, pResult->sign = 1;
}

void multin( sBigNum *pResult , const sBigNum num01 , int n ) {
    tozero(pResult);
    pResult->sign = num01.sign * n / abs(n);
    pResult->size = num01.size;
    n = abs(n);
    int carry = 0;
    for(size_t i = 0; i < num01.size; ++i) {
        pResult->num[i] = num01.num[i] * n + carry;
        carry = pResult->num[i] / 10;
        pResult->num[i] %= 10;
    }
    while(carry != 0) pResult->num[pResult->size++] = carry % 10, carry /= 10;
}

void divide( sBigNum *pQuotient , sBigNum *pRemainder , const sBigNum num01 , const sBigNum num02 ) {
    int index = num01.size - num02.size;
    sBigNum tmp;
    pQuotient->size = (index > 0? index + 1 : 1);
    tozero(pQuotient);
    int eqflag = 0;
    int sign = num01.sign * num02.sign;
    pQuotient->sign = 1;
    sBigNum *ptr = (sBigNum *)&num01;
    ptr->sign = 1;
    ptr = (sBigNum *)&num02;
    ptr->sign = 1;
    for(; index >= 0; --index) {
        for(int i = 1; i <= 9; ++i) {
            pQuotient->num[index] = i;
            multiply(&tmp, *pQuotient, num02);
            if(compare(tmp, num01) == 1) {
                --pQuotient->num[index];
                break;
            }
            if(compare(tmp, num01) == 0) {
                eqflag = 1;
                break;
            }
            if(pQuotient->size - 1 == index && pQuotient->num[index] == 0) --pQuotient;
        }
        if(eqflag == 1) {
            break;
        }
    }
    multiply(&tmp, *pQuotient, num02);
    subtract(pRemainder, num01, tmp);
    if(pQuotient->size <= 0) pQuotient->size = 1;
    pRemainder->sign = sign;
    if(pQuotient->num[pQuotient->size - 1] != 0)pQuotient->sign = sign;
    else if(pQuotient->num[pQuotient->size - 1] == 0) --pQuotient->size;
}

void numtoBigNum(sBigNum *pResult, int64_t num){
    size_t size = 0;
    pResult->sign = num / abs(num);
    while(num != 0) {
        pResult->num[size++] = num % 10;
        num /= 10;
    }
    pResult->size = size;
}

int32_t power(sBigNum *pResult, int32_t n, int32_t k) {
    if(k < 0) return 0;
    sBigNum base;
    numtoBigNum(&base, n);
    tozero(pResult);
    pResult->sign = 1;
    pResult->size = 1;
    pResult->num[0] = 1;
    while(k != 0) {
        if(k & 1) multiply(pResult, *pResult, base);
        multiply(&base, base, base);
        k >>= 1;
    }
    return 1;
}

int32_t combine( sBigNum *pResult, int32_t n, int32_t k ) {
    tozero(pResult);
    pResult->sign = 1;
    pResult->size = 1;
    pResult->num[0] = 1;
    sBigNum malo = {.sign = 1, .size = 1, .num[0] = 1};
    sBigNum test;
    if(k > n || n < 0 || k < 0) return 0;
    for(int i = n - k + 1; i <= n; ++i) {
        multin(pResult, *pResult, i);
    }
    for(int i = 1; i <= k; ++i) multin(&malo, malo, i);
    divide(pResult, &test ,*pResult, malo);
}