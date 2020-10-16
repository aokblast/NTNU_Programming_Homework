#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int day_tab[]={31,28,31,30,31,30,31,31,30,31,30,31};

int isrun(int y){
	if((y%4==0 && y%100!=0) || y%400==0){
		return 1;
	}else{
		return 0;
	}

}

int daycheck(int y,int m,int d){
	if(m==2 && isrun(y)){
		if(d>29){
			return 0;
		}else{
			return 1;
		}
	}else if(y>0 && m>0 && m<13 && d<=day_tab[m-1] && d>=1){
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
	int days=0;	
	y--;
	/*for(int i=1;i<y;i++){
		days+=((y%4==0 && y%100!=0)|| y%400==0) ? 366:365;
	}*/
	days+=365*y+y/4-y/100+y/400;	

	//printf("%d\n",days);
	
	if(m>2){
		days+=(isrun(y)? 1:0);
	}
	
	
	//printf("%d\n",days);
	for(int i=1;i<m;i++){
		days+=(day_tab[i-1]);
	}
	days+=d;
	return days;
}

int minus(int y1,int m1,int d1,int y2,int m2,int d2){
	int a=pass_days(y1,m1,d1);
	int b=pass_days(y2,m2,d2);
	if(isrun(y1) || isrun(y2)){
		b++;
	}
	return b-a;
}


int main(){

	printf("Data Format: YYYY/MM/DD\n");
	int *str_Dat=pas("Start Date: ");
	int *end_Dat=pas("End Date: ");
	printf("%d\n",minus(str_Dat[0],str_Dat[1],str_Dat[2],end_Dat[0],end_Dat[1],end_Dat[2]));
	
}
