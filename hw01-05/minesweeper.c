#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#define i32 int32_t

i32 mine_num=0,width=0,height=0;

typedef struct mine_table{
    i32 status;
    i32 minecnt;
    i32 flag;
    i32 show;
}mine;

int num_check(double num,const char *mode){
    if(num==(int)num){
        if(strcmp(mode,"len")==0){
            if(num>=10 && num<=16){
                return 1;
            }else{
                printf("Wrong input.\n");
                return 0;
            }
        }else if(strcmp(mode,"mine")==0){
            if(num>=30 && num<=90){
                return 1;
            }else{
                printf("Wrong input.\n");
                return 0;
            }
        }else if(strcmp(mode,"mode")==0){
            if(num<=2 && num>=1){
                return 1;
            }else{
                printf("Wrong input.\n");
                return 0;
            }
        }else if(strcmp(mode,"x")==0){
            if(num<width){
                return 1;
            }else{
                printf("Wrong input.\n");
                return 0;
            }
        }else if(strcmp(mode,"y")==0){
            if(num<height){
                return 1;
            }else{
                printf("Wrong input.\n");
                return 0;
            }
        }
    }else{
        printf("Wrong input.\n");
        return 0;
    }
}

void mine_generate(mine *table,size_t num){
    srand(time(NULL));
    for(size_t i=0;i<num;i++){
        i32 x=rand()%width;
        i32 y=rand()%height;
        if((table+y*width+x)->status==1){
            i--;
        }
        (table+y*width+x)->status=1;
    }
}

void display(mine *table,i32 mode){
    printf("  ");
    for(i32 i=0;i<width;i++){
        printf(" %02d",i);
    }
    printf("\n");
    for(int i=-1;i<width;i++){
        printf("---");
    }
    printf("\n");
    for(i32 i=0;i<height;i++){
        printf("%02d|",i);
        for(i32 j=0;j<width;j++){
            mine *ptr=(table+i*width+j);
            if(mode ? (ptr->show) : 1){
                if(ptr->flag==1){
                    printf(" F ");
                }else if(ptr->status==1){
                    printf(" M ");
                }else{
                    printf(" %d ",ptr->minecnt);
                }
            }else{
                printf(" * ");
            }
        }
        printf("\n");
    }
}

void mine_counter(mine *table){
    for(i32 i=0;i<height;i++){
        for(i32 j=0;j<width;j++){
            i32 cnt=0;
            for(i32 k=-1;k<=1;k++){
                for(i32 l=-1;l<=1;l++){
                    if((l!=0 || k!=0) && i+k>=0 && j+l>=0 && i+k<height && j+l<width){
                        if((table+(i+k)*width+j+l)->status==1){
                            cnt++;
                        }
                    }    
                }
            }
           (table+i*width+j)->minecnt=cnt; 
        }
    }
}

i32 input(const char *p,const char *mode){
    double num=0;
    do{
        printf("%s",p);
        scanf(" %lf",&num);
    }while(!num_check(num,mode));
    return (i32)num;
}  

i32 open_check(mine *table){
    for(i32 i=0;i<height;i++){
        for(i32 j=0;j<width;j++){
            mine *ptr=(table+i*width+j);
            if(ptr->status==0){
                if(ptr->show==0){
                    return 0;
                }
            }  
        }
    }
    return 1;
}

i32 flag_check(mine *table){
    i32 cnt=0;
    for(i32 i=0;i<height;i++){
        for(i32 j=0;j<width;j++){
            mine *ptr=(table+i*width+j);
            if(ptr->status==1 && ptr->flag==1){
                cnt++;
            }
        }
    }
    if(cnt==mine_num){
        return 1;
    }else{
        return 0;
    }
}

void open(mine *table,i32 x,i32 y){
    if(x>=0 && y>=0 && x<width && y<height && (table+y*width+x)->status==0 && (table+y*width+x)->show==0){
        if((table+y*width+x)->minecnt==0){
        (table+y*width+x)->show=1;
        open(table,x-1,y-1);
        open(table,x,y-1);
        open(table,x+1,y-1);
        open(table,x-1,y);
        open(table,x+1,y);
        open(table,x-1,y+1);
        open(table,x,y+1);
        open(table,x+1,y+1);
    }else{
        (table+y*width+x)->show=1;
        }
    }
}


void mine_sweeper(mine *table){
    display(table,1);
    i32 option=input("Your Option (1:Open, 2: Flag):","mode");
    double pos[2]={0};
    do{
        printf("Position (row,column):");
        scanf(" %lf %lf",&pos[1],&pos[0]);
    }while(!num_check(pos[0],"x") || !num_check(pos[1],"y"));
    mine *ptr=(table+(i32)pos[0]+(i32)pos[1]*width);
    if(option==1){
        if(ptr->flag==1){
            mine_sweeper(table);
        }
        if(ptr->status==1){
            printf("Game Over!\n");
            exit(0);
       }
        if(ptr->show==0)open(table,(i32)pos[0],(i32)pos[1]);
        if(open_check(table)){
            printf("Congratulation!\n");
            exit(0);
        }
        mine_sweeper(table);
    }else if(option==2){
        ptr->flag=1;
        ptr->show=1;
        if(flag_check(table)){
            printf("Congratulation!\n");
            exit(0);
        }
        mine_sweeper(table);
    }
}

void game_init(){
    width=input("Please input the width  (10-16):","len");
    height=input("Please input the height(10-16):","len");
    mine_num=input("Please input the mine number(30-90):","mine");
    mine *table=(mine* )calloc(width*height,sizeof(mine));
    mine_generate(table,mine_num);
    mine_counter(table);
    mine_sweeper(table);
}
