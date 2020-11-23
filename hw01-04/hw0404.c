#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hw04.h"
#define i32 int32_t

i32 input(const char *p){
    double num=0;
    printf("%s",p);
    if(scanf("%lf",&num)==1 && num>=1 && num<=100 && num==(int)num){
        return (i32)num;
    }else{
        printf("Wrong input.\n");
        exit(0);
    }
}

int main(){
    i32 resistance=input("Please enter the resistance (1-100):");
    i32 num=input("Please enter n (1-100):");
    printf("Ans: %lg\n",eqresis(num,resistance)); 

}
