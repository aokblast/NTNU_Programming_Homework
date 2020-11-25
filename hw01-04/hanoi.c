#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define i32 int32_t

void printmove(i32 disk,i32 rod){
    printf("move disk %d to rod %d.\n",disk,rod);
}

void hanoi_recursive(i32 disk_num,i32 first,i32 end,i32 tmp){
   if(disk_num>0){
        hanoi_recursive(disk_num-1,first,tmp,end);
        printmove(disk_num,end);
        hanoi_recursive(disk_num-1,tmp,end,first);  
    } 
}


