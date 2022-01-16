#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "zeroes.h"
#define i32 int32_t

i32 main(){
    i32 cnt=0,size=sizeof(array)/sizeof(i32);
    for(size_t i=0;i<size-cnt;i++){
        while(array[i]==0 && i!=size-cnt){
            cnt++;
            for(size_t j=i;j<size;j++){
                array[j]=array[j+1];
            }
        }
    }
    for(size_t i=size-cnt;i<size;i++) array[i]=0;
    for(size_t i=0;i<size;i++) printf("%d ",array[i]);
    printf("\n");
}