#include <stdio.h>
#include <stdlib.h>
#include "numeral_sys.h"


int main(){

	uint32_t num=0;

	printf("Please enter a number:");
	if(scanf("%ud",&num)==0){
		printf("Wrong input.\n");
		exit(0);
	
	}

	for(int i=2;i<=16;i++){
		printf("Base %d: %s\n",i,baseN(num,i));
	
	
	}

	//printf("%s",baseN(23,2));
	//baseN(23,12);

}
