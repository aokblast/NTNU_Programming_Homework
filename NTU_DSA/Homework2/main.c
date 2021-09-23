#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void solve() {

}


int main() {
    int testCase = 0;
    scanf("%d", &testCase);
    while(testCase--) {
            int totalPackage = 0, totalOpers = 0, totalLines = 0;
            char operation[10];
            int from = 0, to =  0;
            scanf("%d %d %d", &totalPackage, &totalOpers, &totalLines);
            while(totalOpers--) {
                scanf("%s %d %d", operation, &from, &to);
                if (strcmp(operation, "push") == 0) {

                }else if(strcmp(operation, "merge") == 0) {

                }
            }
    }

    return 0;
}
