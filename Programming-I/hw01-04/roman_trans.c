#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define i32 int32_t

char roman_char_table[13][3]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
i32 number_table[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};

void DecToRoman(i32 num){
    i32 cnt=0;
    while(num>0){
        if(num>=number_table[cnt]){
            printf("%s",roman_char_table[cnt]);
            num-=number_table[cnt];
        }else{
            cnt++;
        }
    }
    printf("\n");
}
