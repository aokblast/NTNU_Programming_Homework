#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t
double factorio(i32 n){
    if(n==1){
        return 1;
    }else{
        return n*factorio(n-1);
    }
}
double natural_log(i32 k){
    if(k<=1){
        return 2;
    }else{
        return 1/factorio(k)+natural_log(k-1); 
    }
} 


