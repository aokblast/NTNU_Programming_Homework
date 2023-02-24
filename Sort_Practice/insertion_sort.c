#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

void isort(i32 *,i32);
void prtarr(i32 *,i32);

int main(){
    i32 arr[]={5,32,1,3,5};
    isort(arr,sizeof(arr)/sizeof(int));
    prtarr(arr,sizeof(arr)/sizeof(int));

}



void isort(i32 *arr,i32 len){
    for(i32 i=0;i<len;i++){
        i32 j=i-1,tmp=arr[i];
        while(j>=0 && arr[j]>=tmp){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=tmp;    
    }
}

void prtarr(i32 *arr,i32 len){
    for(i32 i=0;i<len;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

}
