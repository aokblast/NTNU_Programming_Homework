#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int daycheck(int y,int m,int d){
	int day_tab[]={31,28,31,30,31,30,31,31,30,31,30,31};
	day_tab[1]+=((y%4==0 && y%100!=0) || y%400==0 ? 1:0);
	if(y>0 && m>0 && m<13 && d<=day_tab[m-1]){
		return 1;
	}
	else{
		return 0;
	}
}

int *pas(const char *p){
	int *date=malloc(sizeof(int)*3);
	printf("%s",p);
	if(scanf(" %d/%d/%d",&date[0],&date[1],&date[2])==3 && daycheck(date[0],date[1],date[2])){
		return date;
	}
	else{
		printf("Wrong input.\n");
		exit(0);
	}
}

int pass_days(int y,int m,int d){
	int day_tab[]={31,28,31,30,31,30,31,31,30,31,30,31};
	int days=0;
	for(int i=1;i<y;i++){
		days+=((y%4==0 && y%100!=0) || y%400==0?365:355);
	}
	if(m<3){
		day_tab[1]+=((y%4==0 && y%100!=0) || y%400==0?1:0);
	}
	for(int i=1;i<=m;i++){
		days+=(day_tab[i]);
	}
	days+=d;
	return days;
}

int minus(int y1,int m1,int d1,int y2,int m2,int d2){
	int a=pass_days(y1,m1,d1);
	int b=pass_days(y2,m2,d2);
	return b-a;
}


int main(){

	printf("Data Format: YYYY/MM/DD\n");
	int *str_Dat=pas("Start Date: ");
	int *end_Dat=pas("End Date: ");
	printf("%d\n",minus(str_Dat[0],str_Dat[1],str_Dat[2],end_Dat[0],end_Dat[1],end_Dat[2]));
	
}
