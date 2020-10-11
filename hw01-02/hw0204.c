#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int day_table[]={31,0,31,30,31,30,31,31,30,31,30,31}; 

int *pas(const char *p){
	int *date=malloc(sizeof(int)*3);
	printf("%s",p);
	if(scanf(" %d/%d/%d",&date[0],&date[1],&date[2])==3){
		if(date[1]>0 && date[1]<13){
			if(da)
		}
		printf("%d,%d,%d",date[0],date[1],date[2]);
		return date;
	}
	else{
		printf("Wrong input.\n");
		exit(0);
	}
}


int main(){

	printf("Data Format: YYYY/MM/DD\n");
	int *str_Dat=pas("Start Date: ")-1;
	int *end_Dat=pas("End Date: ")-1;	
	
}
