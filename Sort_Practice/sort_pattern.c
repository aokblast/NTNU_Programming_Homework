#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t

void ptrarr(i32 *,i32);

int main(){

}


void ptrarr(i32 *arr,i32 len){
    for(i32 i=0;i<len;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

}
