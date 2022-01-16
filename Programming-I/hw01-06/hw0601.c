#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ui8 uint8_t
#define i64 int64_t
#define i32 int32_t

i32 input(const char *,const char *);
void bitwise(i64);
void show_hex(i64);

void bitwise(i64 num){
    i32 mode=0,value=0;
    ui8 *ptr_num=0;
    ptr_num=&num;
    while(1){
        printf("The integer: %ld\n",num);
        show_hex(num);
        do{
            mode=input("Please enter the postition: (1-8,0: End): ","mode");
            if(mode == 0) exit(0);
        }while(mode == -1);
        do{
            value=input("Please enter the new value (0-255): ","value");
        }while(value == -1);
        *(ptr_num+mode-1)=value;
    
    }
}

void show_hex(i64 num){
    ui8 *ptr_num=0;
    ptr_num=&num;
    for(size_t i=0;i<8;i++){
        printf("(%lu) 0x%02X ",i+1,*(ptr_num+i));
    }
    printf("\n");
}

i32 input(const char *msg,const char *mode){
    printf("%s",msg);
    double num=0;
    if(scanf("%lf",&num)==0 || num!=(i32)num){
        printf("Wrong input.\n");
        return -1;
    }
    if(strcmp(mode,"mode")==0){
        if((i32)num<=8 && (i32)num>=0){
            return (i32)num;
        }else{
            printf("Wrong input.\n");
            return -1;
        }
    }else if (strcmp(mode,"value")==0){
        if((i32)num>=0 && (i32)num<=255){
            return num;
        }else{
            printf("Wrong input.\n");
            return -1;
        }
    }
}

int main(){
    i64 num=0;    
    printf("Please input an integer:");
    if(scanf("%ld",&num)!=1){
        printf("Wrong input.");
    }
    bitwise(num);

}
