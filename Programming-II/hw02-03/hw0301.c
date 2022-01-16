#include "all.h"
#include "lrc.h"

int main(){
    char fileName[200];
    printf("Please enter a LRC file:");
    scanf(" %s", fileName);
    lrcobj *song1 = read_lrc(fileName);
    
    if(song1 == NULL) {
        printf("Wrong input.\n");
        return 0;
    }
    setting_the_fucking_color(song1, 1);
    play_lrc(song1);

}