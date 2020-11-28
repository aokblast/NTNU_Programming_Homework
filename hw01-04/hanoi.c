#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define i32 int32_t

i32 hanoi_stack[10000][5];
i32 cnt=-1;

void printmove(i32 disk,i32 rod){
    printf("move disk %d to rod %d.\n",disk,rod);
}
void pop(){
    printmove(hanoi_stack[cnt][0],hanoi_stack[cnt][2]);
    cnt--;
    //printf("%lu",cnt);
}
void top(){
    printf("%d,%d,%d,%d\n",hanoi_stack[cnt][0],hanoi_stack[cnt][1],hanoi_stack[cnt][2],hanoi_stack[cnt][3]);
}
void push(i32 num,i32 a,i32 b,i32 c,i32 status){
    cnt++;
    hanoi_stack[cnt][0]=num;       
    hanoi_stack[cnt][1]=a;
    hanoi_stack[cnt][2]=b;
    hanoi_stack[cnt][3]=c;
    hanoi_stack[cnt][4]=status;
    //printf("%lu\n",cnt);
}
i32 isEmpty(){
    if(cnt<0){
        return 1;
    }else{
        return 0;
    }
}

void hanoi_recursive(i32 disk_num,i32 first,i32 end,i32 tmp){
   if(disk_num>0){
        hanoi_recursive(disk_num-1,first,tmp,end);
        printmove(disk_num,end);
        hanoi_recursive(disk_num-1,tmp,end,first);  
    } 
}

void hanoi_loop(i32 disk_num,i32 first,i32 end,i32 tmp){
    push(disk_num,first,end,tmp,0);
    do{
        disk_num=hanoi_stack[cnt][0],first=hanoi_stack[cnt][1],end=hanoi_stack[cnt][2],tmp=hanoi_stack[cnt][3];
        i32 status=hanoi_stack[cnt][4];
        cnt--;
        if(status==1){
            printmove(disk_num,end);
        }else{
            push(disk_num-1,tmp,end,first,(disk_num-1<=1));
            push(disk_num,first,end,tmp,1);
            push(disk_num-1,first,tmp,end,(disk_num-1<=1));
            disk_num=hanoi_stack[cnt][0];
        }    
    }while(!isEmpty());
} 
