#include "all.h"
#include "myprintf.h"

int main(){
    printf("%#+-23X123\n", -123);
    myprintf("%#+-23X123\n", -123);
}