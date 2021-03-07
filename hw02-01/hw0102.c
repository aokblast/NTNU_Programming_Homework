#include <stdio.h>
#include <stdlib.h>
#include "strlib.h"
#include <stdbool.h>

bool issubstr(const char target[], const char query[]) {
    for(size_t i = 0; query[i] != '\0'; ++i) {
        //printf("%c,%c", target[i], query[i]);
        if(query[i] != target[i]) return false;
    }
    return true;

}

void printbefore(const char target[], const char query[]) {
    for(size_t i = 0; target[i] != '\0'; ++i) {
        if(target[i] == query[0] && issubstr(&target[i], query)) {
            printf("\033[34m");
            size_t j = 0;
            for(j = 0; query[j] != '\0'; ++j) {
                printf("%c", target[i + j]);
            }
            printf("\033[0m");
            i += (j - 1);
        }else {
            printf("%c", target[i]);
        }
    }

}

void printafter(const char target[], const char query[], const char repl[]) {
    for(size_t i = 0; target[i] != '\0'; ++i){
        if(target[i] == query[0] && issubstr(&target[i], query)) {
            printf("\033[31m");
            int size = 0;
            for(size = 0; query[size] != '\0'; ++size);
            for(size_t j = 0; repl[j] != '\0'; ++j) {
                printf("%c", repl[j]);
            }
            printf("\033[0m");
            i += (size - 1);
        }else {
            printf("%c", target[i]);
        }

    }

}



int main(){
    char text[1024] = {0}, query[128] = {0}, repl[128] = {0};
    printf("Please enter the original text:\n");
    getaline(text, 1024);
    printf("Keyword:\n");
    getaline(query, 128);
    printf("New word:\n");
    getaline(repl, 128);
    printf("Before:\n");
    printbefore(text, query);
    printf("\n");
    printf("After:\n");
    printafter(text, query, repl);
    printf("\n");
}