#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t

void ptrarr(i32 *,i32);
void swap(i32 *,i32 *);
void msort(i32 *,i32,i32);

int main(){

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

void msort(i32 *arr,i32 head,i32 end){
    if(head<end){
        i32 mid=(head+end)/2;
        msort(arr,head,mid);
        msort(arr,mid+1,end);
        merge(arr,head,mid,end);
    }
}
