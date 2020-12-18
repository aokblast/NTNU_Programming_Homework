#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t

void ptrarr(i32 *,i32);
void swap(i32 *,i32 *);
void quicksort(i32 *,i32,i32);
i32 partition(i32 *,i32,i32);

int main(){
    int arr[]={1,245,23,4,1};
    quicksort(arr,0,sizeof(arr)/sizeof(i32)-1);
    ptrarr(arr,sizeof(arr)/sizeof(i32));
}

void quicksort(i32 *arr,i32 front,i32 end){
    if(front<end){
        i32 pivot=partition(arr,front,end);
        quicksort(arr,front,pivot-1);
        quicksort(arr,pivot+1,end);
    }
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

i32 partition(i32 *arr,i32 front,i32 end){
    i32 i=front-1,pivot=arr[end];
    for(i32 j=front;j<end;j++){
        if(arr[j]<pivot){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    i++;
    swap(&arr[i],&arr[end]);
    return i;

}
