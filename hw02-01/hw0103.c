#include <stdio.h>
#include <stdlib.h>
#include "hw0103.h"


void swap(char **p1, char **p2) {
    char *tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}


int main(){
    print_answers();
    swap(&pStr01, &pStr02);
    print_answers();
}