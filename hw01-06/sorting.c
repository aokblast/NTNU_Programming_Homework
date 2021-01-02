#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t
#define u8 uint8_t

i32 partition(u8 *,i32,i32,i32(*)(i32,i32));
void quicksort(u8 *,i32,i32,i32(*)(i32,i32));

void print_card(const u8 player[13]){
    for(size_t i=0;i<13;i++) printf("%u ",player[i]);
    printf("\n");
}

i32 func01(i32 a,i32 b){
    a=(a-1)%13+1;
    b=(b-1)%13+1;
    return a>b; 
}

i32 func02(i32 a,i32 b){
    b=(b-1)%13+1;
    a=(a-1)%13+1;
    if(b==2&&a==2) return 0;
    else if(b==2 && a!=2) return 0;
    else if(b!=2 && a==2) return 1;
    else return a>b;
}

i32 func03(i32 a,i32 b){
    i32 a_col=(a-1)/13;
    i32 b_col=(b-1)/13;
    i32 a_val=(a-1)%13+1;
    i32 b_val=(b-1)%13+1;
    if(a_col<b_col) return 1;
    else if(a_col>b_col) return 0;
    else return a_val>b_val;
}

i32 sort_card(u8 player[13],i32 (*compare)(i32 a,i32 b)) {
    quicksort(player,0,12,compare);
    return 1; 
}

void quicksort(u8 *arr,i32 front,i32 end,i32 (*compare)(i32 a,i32 b)){
    if(front<end){
        i32 pivot=partition(arr,front,end,compare);
        quicksort(arr,front,pivot-1,compare);
        quicksort(arr,pivot+1,end,compare);
    }
}

void swap(u8 *a,u8 *b){
    u8 tmp=*a;
    *a=*b;
    *b=tmp;
}

i32 partition(u8 *arr,i32 front,i32 end,i32 (*compare)(i32 a,i32 b)){
    i32 i=front-1,pivot=arr[end];
    for(i32 j=front;j<end;j++){
        if(compare(arr[j],pivot)){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    i++;
    swap(&arr[i],&arr[end]);
    return i;
}
