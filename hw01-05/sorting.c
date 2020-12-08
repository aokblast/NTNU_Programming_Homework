#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define i32 int32_t

void swap(i32 *a,i32 *b){
    i32 tmp=*a;
    *a=*b;
    *b=tmp;
}

i32 partition(i32 *arr,i32 front,i32 end,i32 mode){
    i32 i=front-1,pivot=arr[end];   
    for(i32 j=front;j<end;j++){
    if(mode ? arr[j]>pivot : arr[j]<pivot){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    i++;
    swap(&arr[i],&arr[end]);
    return i;
}

void quick_sort(i32 *arr,i32 front,i32 end,i32 mode){
    if(front<end){
        i32 pivot=partition(arr,front,end,mode);
        quick_sort(arr,front,pivot-1,mode);
        quick_sort(arr,pivot+1,end,mode);
    }
}


void init(i32 *arr,i32 arr_size){
    i32 arr1[100000]={0},a=0,b=0,arr2[10000]={0};
    for(int i=0;i<arr_size;i++){
        if(abs(arr[i])%2==0){
            arr1[a]=arr[i];
            a++;
        }else{
            arr2[b]=arr[i];
            b++;
        }
    }
    quick_sort(arr1,0,a-1,1);
    quick_sort(arr2,0,b-1,0);
    for(i32 i=0;i<b;i++){
        printf("%d ",arr2[i]);
    }
    for(i32 i=0;i<a;i++){
        printf("%d ",arr1[i]);
    }
    printf("\n");
    
}
