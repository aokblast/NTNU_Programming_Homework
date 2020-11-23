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

void hanoi_loop(i32 disk_num){
    i32 cnt=0,ho[1000][3],tmp=3,target=2,init=1;
    memset(ho,0,sizeof(ho));
    for(i32 i=disk_num;i>=1;i--){
        ho[cnt][0]=disk_num;
        cnt++;
    }
    for(i32 i=0;i<3*disk_num-2;i++){
        
    }
}
