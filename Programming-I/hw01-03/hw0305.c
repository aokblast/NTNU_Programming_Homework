#include <stdio.h>
#include <stdlib.h>

int input(const char *n){
    double num;
    printf("%s",n);
    if(scanf("%lf",&num)==1 && num>0 && num==(int)num){
        return (int)num;
    }else{
        printf("Wrong input.\n");
        exit(0);
    }
}

int *arr_input(const char *n){
    int *arr_num=malloc(sizeof(int)*5);
    printf("%s",n);
    if(scanf("%d,%d,%d,%d,%d",&arr_num[0],&arr_num[1],&arr_num[2],&arr_num[3],&arr_num[4])==5){
        for(int i=0;i<5;i++){
            if(arr_num[i]<0){
                printf("Wrong input.\n");
                exit(0);
            }
        }
    }else{
        printf("Wropng input.\n");
        exit(0);
    }
    return arr_num;
}


int main(){
    int ann_pay=0,*ins_fee,fee_off=0,proc_fee=0,age=0,pay_year=0,exp_annret=0,cnt=0;
    double total=0;
    printf("SIP Plan\n");
    ann_pay=input("Annually payment : ");
    ins_fee=arr_input("Insurance fee in the first five years (0 is assumed afterwards ):\n");
    fee_off=input("Insurance fee off: ");
    proc_fee=input("Monthly insurance processing fee: ");
    age=input("Age : ");
    pay_year=input("How many years of payment : ");
    exp_annret=input("Expected annual return on investment rate : ");
    printf("------------------------------\n");
    printf("Your Payment and Account Value Table\n");

    for(int i=age;i<=100;i++){
        if(i<age+pay_year){
            total+=ann_pay*(1-(double)(i>=age+5 ? 0 : ins_fee[i-age])/100+(double)(i>=age+5 ? 0 : fee_off)/100);
            cnt++;
        }
        total-=(age>20 ? 100*i:2000);
       // printf("%lf ",total);
        for(int j=1;j<=(i==age ? 11:12);j++){
            total-=proc_fee;
            total*=(1+(double)exp_annret/1200);
            //printf("%d,%lf ",j,total);
        }
        printf("%d: %d,%lf\n",i,ann_pay*cnt,total);
    
    }


}
