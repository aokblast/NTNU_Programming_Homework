#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int htd(const char hex[]) {
    int result = 0;

    for(size_t i = 0; i < 2; ++i) {
        char tmp = hex[i];
        result = result * 16 + (isdigit(tmp) ? tmp - '0' : toupper(tmp) - 'A' + 10);
    }

    return result;

}

int getaline(char target[], int buffer) {
    int i, c;
    for (i = 0; (c = getchar()) && c != '\n' && c != EOF && i < buffer - 1; ++i) target[i] = c;
    target[i] = '\0';
    if(i % 2 == 1) return 0;
    else return 1;
}

void hts(const char target[], char to[]) {
    int j = 0;
    for (size_t i = 0; target[i] != '\0'; i += 2) {
        int tmp = htd(&target[i]);
        //printf("%d\n", tmp);
        if(tmp >= 32 && tmp <= 127) {
            to[j++] = tmp;
        }else if (tmp == 0) {
            break;            
        }else{
            printf("Invalid input");
            return;
        }
    }
    to[j] = 0;

}

int main(){
    char str[100000] = {0};
    char ans[100000] = {0};
    printf("Please enter the hex string: ");
    if(getaline(str, 100000)){
        hts(str, ans);
        printf("%s\n", ans);
    }else{
        printf("Invalid input\n");

    }



}