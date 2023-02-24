#include <stdio.h>
#include <stdlib.h>
#define i32 int32_t
#define MAXSTACK 10000
i32 stack[MAXSTACK];
i32 top=-1;

i32 IsEmpty();
void push();
void pop();
i32 gettop();
i32 getSize();
void push(i32 num){
    if(top==MAXSTACK){
        printf("This stack is full!\n");
    }else{
        top++;
        stack[top]=num;
    }
}

void pop(){
    if(IsEmpty()){
        printf("Nothing can be pop!\n");
    }else{
        printf("%d",stack[top]);
        top--;
    }
}
i32 gettop(){
    if(IsEmpty()){
        printf("Empty!\n");
        return -1;
    }else{
        return top;
    }

}
i32 getSize(){
    if(IsEmpty()){
        printf("Empty!\n");
    
    }
    return top+1;

}
i32 IsEmpty(){
    if(top==-1){
        return 1;
    }else{
        return 0;
    }

}

int main(){
    push(5);
    pop();
    push(1);
    push(2);
    push(3);
    printf("%d\n",gettop());
    printf("%d\n",getSize());

}
