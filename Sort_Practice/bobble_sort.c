#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t

void ptrarr(i32 *,i32);
void swap(i32 *,i32 *);
void bsort(i32 *,i32);

int main(){
    i32 arr[]={1,5,3,2,2,1};
    bsort(arr,sizeof(arr)/sizeof(i32));
    ptrarr(arr,sizeof(arr)/sizeof(i32));
}


void ptrarr(i32 *arr,i32 len){
    for(i32 i=0;i<len;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void swap(i32 *a,i32 *b){
    i32 tmp=*a;
    *a=*b;
    *b=tmp;
}

void bsort(i32 *arr,i32 len){
    if(len==1){
        return ;
    }
    for(int i=0;i<len-1;i++){
        if(arr[i]>arr[i+1]){
            swap(&arr[i],&arr[i+1]);
        }
    }

    return bsort(arr,len-1);

}
