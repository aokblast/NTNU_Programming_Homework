#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

double eqresis(i32 n,i32 r){
    if(n==1){
        return r+r;
    }else{
        return eqresis(n-1,r)*r/(eqresis(n-1,r)+r/1.0)+r;
    }
}


