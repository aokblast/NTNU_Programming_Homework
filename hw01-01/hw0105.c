#include <stdio.h>
#include <stdint.h>
int main(){
	int32_t num1;
	printf("Please enter an integer: ");
	scanf("%d",&num1);
	printf("%d: %08x\n",num1,num1);
}
