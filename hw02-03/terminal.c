#include "all.h"
#include "san5pk_editor.h"
#include "terminal.h"

void init_term() {
    //file read and check
     
    /*
    string fileName;
    printf("Please enter the file you want to edit.\n");
    scanf(" %s", fileName);
    if(strcmp(&fileName[strlen(fileName) - 4], ".S5P") != 0) {
        printf("Wrong file.\n");
        return;
    }
    */
    fstream readFile = fopen("/home/blast/sanguo/SAVEDATA.S5P", "rb");
    if(readFile == NULL) {
        printf("File not exist.\n");
        return;
    }

    //exec
    int mode = 0;
    printf("Please enter the mode you want to edit (1. Monarch, 2. City, 3. Commander, 0. Exit):");
    while (scanf(" %d", &mode)) {
        switch(mode) {
            case 1:
                edit_monarch(readFile);
                break;
            case 2:
                edit_city(readFile);
                break;
            case 3:
                edit_commander(readFile);
                break;
            case 0:
                printf("Good Bye.\n");
                return ;
            default:
                printf("Wrong mode, Please retry.\n");
                break;
        }
        printf("Please enter the mode you want to edit (1. Monarch, 2. City, 3. Commander, 0. Exit):");
    }

    fclose(readFile);
}