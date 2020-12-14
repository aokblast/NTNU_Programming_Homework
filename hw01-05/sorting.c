#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define i32 int32_t

void swap(i32 *a,i32 *b){
    i32 tmp=*a;
    *a=*b;
    *b=tmp;
}

i32 cmp(i32 a,i32 b){
    i32 x=abs(a);
    i32 y=abs(b);
    if(x%2==0 && y%2==0){
        return a>b;    
    }else if(x%2==1 && y%2==0){
        return 1;
    }else if(x%2==0 && y%2==1){
        return 0;
    }else{
        return a<b;
    }
}

i32 partition(i32 *arr,i32 front,i32 end,i32 (*cmp)(i32,i32)){
    i32 i=front-1,pivot=arr[end];   
    for(i32 j=front;j<end;j++){
    if(cmp(arr[j],pivot)){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    i++;
    swap(&arr[i],&arr[end]);
    return i;
}

void quick_sort(i32 *arr,i32 front,i32 end,i32 (*cmp)(i32,i32)){
    if(front<end){
        i32 pivot=partition(arr,front,end,cmp);
        quick_sort(arr,front,pivot-1,cmp);
        quick_sort(arr,pivot+1,end,cmp);
    }
}


void sorting(i32 *arr,i32 arr_size){
    quick_sort(arr,0,arr_size-1,cmp);
    for(i32 i=0;i<arr_size;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    
}
