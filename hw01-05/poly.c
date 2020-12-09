#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

struct array{
    i32 degree;
    i32 *arr;
};
struct array poly_input(const char *str){
    struct array result;
    printf("Please enter %s degree:",str);
    scanf(" %d",&result.degree);
    result.arr=(i32 *)malloc(sizeof(i32)*(result.degree+1));
    printf("Please enter %s coefficient:",str);
    for(i32 i=result.degree;i>=0;i--){
        scanf(" %d",(result.arr+i));
    }
    return result;
}

void poly_print(struct array arr1){
    if(*(arr1.arr+arr1.degree)!=0)printf("%dx^%d",*(arr1.arr+arr1.degree),arr1.degree);
    for(i32 i=arr1.degree-1;i>0;i--){
        if(*(arr1.arr+i)!=0)printf("+%d^x%d",*(arr1.arr+i),i);
    }
    if(*arr1.arr!=0)printf("+%d",*arr1.arr);
    if(arr1.degree==0 && *(arr1.arr)==0)printf("0");
    printf("\n");
}

struct array poly_plus(struct array arr1,struct array arr2,i32 mode){
    i32 max_len=(arr1.degree>arr2.degree?arr1.degree:arr2.degree);
    struct array result;
    result.arr=(i32 *)malloc(sizeof(i32)*(max_len+1));
    for(i32 i=0;i<=max_len;i++){
        i32 a=(i<=arr1.degree?*(arr1.arr+i):0);
        i32 b=(i<=arr2.degree?*(arr2.arr+i):0);
        *(result.arr+i)=(mode?a+b:a-b);
    }
    while(*(result.arr+max_len)==0 && max_len>0)max_len--;
    result.degree=max_len;
    return result;
}

struct array poly_multi(struct array arr1,struct array arr2){
    i32 max_len=arr1.degree+arr2.degree;
    struct array result;
    result.arr=(i32 *)malloc(sizeof(i32)*(max_len+1));
    for(i32 i=0;i<=arr1.degree;i++){
        for(i32 j=0;j<=arr2.degree;j++){
            *(result.arr+i+j)+=(*(arr1.arr+i))*(*(arr2.arr+j));
        }
    }
    result.degree=max_len;
    return result;
}

void poly(){
    struct array p1=poly_input("p1");
    struct array p2=poly_input("p2");
    printf("p1:");
    poly_print(p1);
    printf("p2:");
    poly_print(p2);
    struct array plus=poly_plus(p1,p2,1);
    struct array minus=poly_plus(p1,p2,0);
    printf("p1+p2:");
    poly_print(plus);
    printf("p1-p2:");
    poly_print(minus);
    struct array multi=poly_multi(p1,p2);
    printf("p1*p2:");
    poly_print(multi);
    free(p1.arr);
    free(p2.arr);
    free(plus.arr);
    free(minus.arr);
    free(multi.arr);
}
