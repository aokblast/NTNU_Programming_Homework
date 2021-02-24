#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXLEN 256

typedef enum {
	EQ = 0,NEQ,AB,LW
}status;

typedef struct {
	int *content;
	size_t len;
} BigInt;


BigInt getIntArr(char *num, size_t len) {
	BigInt ret;
	ret.len = len;
	ret.content = calloc(sizeof(int), len);
	for (size_t i = 0; i < len; i++) ret.content[i] = num[len - i - 1] - '0';
	return ret;
}

void print(BigInt num) {
	for(size_t i = num.len - 1; i != -1; --i) {
		printf("%d", num.content[i]);
	}
	printf("\n");

}

bool checkZero(BigInt num) {
	for (size_t i = 0; i < num.len; ++i) if(num.content[i]) return false;
	return true;
}

bool checkEven(BigInt num) {
	if (num.content[0] % 2 == 0) return true;
	return false;
}

BigInt divide(BigInt num) {
	bool carry = 0;
	for(int i = num.len - 1; i != -1; --i) {
			int tmp = num.content[i];
			num.content[i] = (num.content[i] + carry * 10) / 2;
			carry = tmp % 2;
	}
	while(num.content[num.len - 1] == 0) --num.len;
	if(num.len == 0) num.len = 1;
   	return num;	
}

BigInt sub(BigInt num1, BigInt num2) {
	bool carry = 0;
	for (size_t i = 0; i < num1.len; ++i) {
		if(i < num2.len) {
			num1.content[i] -= (num2.content[i] + carry);
			if(num1.content[i] < 0) {
				num1.content[i] += 10;
				carry = 1;
			}else {
				carry = 0;
			}
		}else if(carry == 1){
			if(num1.content[i] == 0) {
				num1.content[i] = 9;
			}else {
				num1.content[i] -= 1;
				carry = 0;
			}
		}
	}
	while(num1.content[num1.len - 1] == 0) --num1.len;
	if(num1.len == 0) num1.len = 1;
	return num1;
}



void swap(BigInt *num1, BigInt *num2) {
	BigInt tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}

status compare(BigInt num1, BigInt num2) {
	if(num1.len > num2.len) return AB;
	else if(num1.len < num2.len) return LW;
	else {
		for(size_t i = num1.len - 1; i != -1; --i) {
			if(num1.content[i] > num2.content[i]) return AB;
			else if(num1.content[i] < num2.content[i]) return LW;
		}
	}
	return EQ;
}

BigInt multi(BigInt num, uint64_t ans){
	BigInt ret;
	int carry = 0;
	ret.content = calloc(sizeof(int), 257);
	for(ret.len = 0; ret.len < num.len; ret.len++) {
		ret.content[ret.len] = num.content[ret.len] * ans + carry;
		carry = ret.content[ret.len] / 10;
		ret.content[ret.len] %= 10;
	}
	return ret;
}


int main (){
	char num[MAXLEN] = {0};
	BigInt num1 , num2, answer;
	uint64_t ans = 1;
	scanf("%s", num);
	num1 = getIntArr(num, strlen(num));
	scanf("%s", num);
	num2 = getIntArr(num, strlen(num));

	if(compare(num1, num2) == LW) swap(&num1, &num2);
	while(!checkZero(num1) && !checkZero(num2)) {
		if(checkEven(num1) && checkEven(num2) ) {
			num1 = divide(num1);
			num2 = divide(num2);
			ans *= 2;
		}else if (checkEven(num2)) {
			num2 = divide(num2);
		}else if (checkEven(num1)) {
			num1 = divide(num1);
		}
		if(compare(num1, num2) == LW) swap(&num1, &num2);
		num1 = sub(num1, num2);

	}
	answer = multi(num2, ans);
	print(answer);
}
