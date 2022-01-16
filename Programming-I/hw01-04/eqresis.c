#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

double eqresis(i32 n,i32 r){
    if(n==1){
        return r*2.0;
    }else{
        double k=eqresis(n-1,r);
        return k*r/(k+r)+r;
    }
}


