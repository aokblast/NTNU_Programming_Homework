#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define i32 int32_t

i32 offset=0,arr[10]={0},cnt=0;

void add(i32 val){
    if(offset<0){
	if(cnt+offset>=0) cnt+=offset;
	else cnt=0;
        offset=0;
    }
    if(cnt>=10){
        for(i32 i=1;i<10;i++){
            arr[i-1]=arr[i];
        }
        arr[9]=val;
        return ;
    }
    arr[cnt]=val;
    cnt++;
}

void redo(){
    if(offset<0) offset++;
}

void undo(){
    if(cnt==0 || cnt+offset <=0) return;
    if(offset>-10) offset--;
}

void ptr(){
    for(i32 i=0;i<cnt+offset;i++)printf("%d ",arr[i]);
    printf("\n");
}

void texteditor(){
    double val=0;
    printf("input:");
    while(scanf("%lf",&val)==1 && val!=0 && val==(i32)val && val>=-2){
        if(val==-1)undo();
        else if(val==-2)redo();
        else add(val);
    }
    ptr();
}
