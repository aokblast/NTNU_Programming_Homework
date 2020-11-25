#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hw04.h"
#define i32 int32_t

int input(const char *p){
    double num=0;
    printf("%s",p);
    if(scanf("%lf",&num)==1 && num>=1 && 3000>=num && num==(int)num){
        return (int)num;
    }else{
        printf("Wrong input.\n");
        exit(0);
    }

}

int main(){
    int num=input("Please input a number: ");
    DecToRoman(num);

}
