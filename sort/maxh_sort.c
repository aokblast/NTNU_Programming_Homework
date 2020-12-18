#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define i32 int32_t

void ptrarr(i32 *,i32);
void heaplify(i32 *,i32,i32);
void swap(i32 *,i32 *);
void buildmaxheap(i32 *,i32);
void maxhsort(i32 *,i32);
int main(){
    i32 arr[]={5,1,3,56,6,1,2,6,7};
    maxhsort(arr,sizeof(arr)/sizeof(i32));
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

void heaplify(i32 *arr,i32 root,i32 len){
    i32 left=2*root+1,right=2*root+2;
    i32 largest_index=root;
    if(left<len && arr[left]>arr[largest_index]){
        largest_index=left;
    }
    if(right<len && arr[right]>arr[largest_index]){
        largest_index=right;
    }
    //i32 largest_index=((arr[left]>arr[root] && left<len) ? left : ((arr[right]>arr[root] && right<len) ?right:root));
    if(largest_index!=root){
        swap(&arr[largest_index],&arr[root]);
        heaplify(arr,largest_index,len);
    }
}

void buildmaxheap(i32 *arr,i32 len){
    for(i32 i=len/2-1;i>=0;i--){
        heaplify(arr,i,len);
    }
}

void maxhsort(i32 *arr,i32 len){
    buildmaxheap(arr,len);
    for(i32 i=len-1;i>=0;i--){
        swap(&arr[0],&arr[i]);
        heaplify(arr,0,i);
    
    }  
}
