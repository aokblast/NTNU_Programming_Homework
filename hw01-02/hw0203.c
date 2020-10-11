#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

struct tax{
	double_t rate;
	int32_t pdc;
}class[5]={{0.05,0},{0.12,37800},{0.2,134600},{0.3,376600},{0.4,829600}};

struct profile{
	int32_t salary,marry,pop,income_pop,sav_inv,tuit,child,disabled;
}person;

//Print and Scanf
int32_t pas(const char *p){
	int32_t num=0;
	printf("%s",p);
	
	if(scanf(" %d",&num)==1){
		return num;
	}
	else{
		printf("Wrong input.\n");
		exit(0);
	}
}

int32_t res(int32_t total,struct tax group){
	return printf("%lg\n",total*group.rate-group.pdc);

}

int main(){
	person.salary=pas("Gross salary: ");
	person.marry=pas("Married? (0/1): ");
	if(person.marry!=1 && person.marry!=0){
		printf("Wrong input\n");
		exit(0);
	}
	person.pop=pas("How many people are there in your family: ");
	person.income_pop=pas("How many people in your family have income: ");
	person.sav_inv=pas("Special Deduction for Savings and Investment: ");
	person.tuit=pas("Special Deduction for Tuition: ");
	person.child=pas("How many preschool children: ");
	person.disabled=pas("How many disable people: ");
	
	int32_t total=person.salary-person.pop*88000-person.income_pop*200000-(person.marry? 240000:120000)-person.sav_inv*270000-person.tuit*25000-person.child*120000-person.disabled*2000000;
	
	//printf("%d\n",total);
	
	if(total>4530000){
		res(total,class[4]);
	}else if(total>2420000){
		res(total,class[3]);
	}else if(total>1210000){
		res(total,class[2]);
	}else if(total>540000){
		res(total,class[1]);
	}else{
		res(total,class[0]);
	}


}
