#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t
i32 factorio(i32 n){
    if(n==1){
        return 1;
    }else{
        return n*factorio(n-1);
    }
}
double natural_log(i32 k){
    if(k<=1){
        return 2.0;
    }else{
        return 1.0/factorio(k)+natural_log(k-1); 
    }
} 


