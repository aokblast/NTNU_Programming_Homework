#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 1000000

typedef struct 
{
    double cont[MAXLEN];
    int top;  
} stack;

int top(stack *stk) {
    return stk->top;
}

bool isempty(stack *stk) {
    if(stk->top == -1) return true;
    return false;
}

void push(stack *stk, double n) {
    stk->cont[++stk->top] = n;
}

double pop(stack *stk) {
    return stk->cont[stk->top--];
}

double peek(stack *stk) {
    return stk->cont[stk->top];
}

void print(stack *stk) {
    for(int i = stk->top; i >= 0; --i) {
        printf("%d, %lf\n", i, stk->cont[i]);
    }
}

stack numstack = {{0}, -1};
stack operstack = {{0}, -1};
char precedence[3][3] = {"+-","*/", "()"};
char operator[] = "+-*/()";

int checkpre(char operator) {
    for(int i = 0; i < 3; ++i) {
        if(strchr(precedence[i], operator) != NULL) return i;
    }
    return -1;
}

double operate(double num1, double num2, char operator) {
    switch(operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
    }
}

double solve(char str[]) {
    int num = 0;
    double result = 0;
    bool inputed = false;
    bool imp = false;
    for(int i = 0; str[i] != '\n' && str[i] != '\0'; ++i) {
        /*
        print(&numstack);
        printf("str[i]:%c\n", str[i]);
        for(int i = 0; i <= operstack.top; ++i) printf("%d, %c\n", i, (char)operstack.cont[i]);
        */
        if(isdigit(str[i])) inputed = true, num = num * 10 + str[i] - '0';
        else if(strchr(operator, str[i]) != NULL) {
            
            if(inputed) inputed = false,push(&numstack, num);
            num = 0;
            if(str[i] == ')') {
                while((char)peek(&operstack) != '(') push(&numstack, operate(pop(&numstack), pop(&numstack), (char)pop(&operstack)));
                pop(&operstack);
                while(strchr(operator, str[i]) == NULL) imp = true,++i;
                if(imp) imp = false, --i;
                continue;
            }
            while(!isempty(&operstack) && (char)peek(&operstack) != '('  && checkpre((char)peek(&operstack)) >= checkpre(str[i])) push(&numstack, operate(pop(&numstack), pop(&numstack), (char)pop(&operstack)));
            push(&operstack, str[i]);
        }
    }

    if(inputed) inputed = false, push(&numstack, num);
    while(!isempty(&operstack)) push(&numstack, operate(pop(&numstack), pop(&numstack), pop(&operstack)));
    return pop(&numstack);
}

int main(){
    char str[MAXLEN];
    while(fgets(str, MAXLEN, stdin) != NULL) {
        while(!isempty(&operstack)) pop(&operstack);
        while(!isempty(&numstack)) pop(&numstack);
        double result = solve(str);
        printf("%.15lf\n", result);
    }
}