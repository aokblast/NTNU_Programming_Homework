#include "all.h"

void random_length_string(char *str, size_t length) {
    for(int i = 0; i < length; ++i) {
        str[i] = rand() % 26 + 'a';
    }
}

int in_variables_index(char str[], char variable[][100]) {
    for(int i = 0; i < 10000; ++i) {
        if(strcmp(str, variable[i]) == 0) {
            return i;
        }else if (variable[i][0] == '\0') {
            return -1;
        }
    }
    return -1;
}

bool is_function(char str[]) {
    return (strrchr(str, '(') != NULL && strrchr(str, ')') != NULL);
}


void obfuscation(fstream readFile, fstream writeFile, int mode) {
    char variables[10000][100] = {0}, replacement[10000][100] = {0};
    string word;
    int top = 0;
    //skip pre-processor
    while(!feof(readFile)) {
        uint64_t lastPos = ftell(readFile);
        fgets(word, 10005, readFile);
        if(strncmp(word, "int", 3) == 0 || strncmp(word, "char", 4) == 0) {
            fseek(readFile, lastPos, SEEK_SET);
            break;
        }else {
            fwrite(word, 1, strlen(word), writeFile);
        }
    }

    //obfuscation
    while(!feof(readFile)) {
        uint64_t lastPos = ftell(readFile);
        fgets(word, 10005, readFile);
        bool hasInter = false;
        if(word[0] == '\n') {
            continue;
        }else {
            fseek(readFile, lastPos, SEEK_SET);
        }
        fscanf(readFile, " %s", word);
        if(word[strlen(word) - 1] == ';') {
            word[strlen(word) - 1] = '\0';
            hasInter = true;
        }
        int index = in_variables_index(word, variables);
        char redun[1] = {((rand() % 2 ) == 0 ? ' ' : '\n')};
        if(mode >= 1) {
            fwrite(redun, 1, sizeof(char), writeFile);
        }
        if(strcmp(word, "char") == 0 || strcmp(word, "int") == 0) {
            fwrite(word, 1, strlen(word), writeFile);
            redun[0] = ((rand() % 2 ) == 0 ? ' ' : '\n');
            hasInter = false;
            if(mode >= 1) {
                fwrite(redun, 1, sizeof(char), writeFile);
            }
            fscanf(readFile, " %s", word);
            if(mode >= 3) {

                if(is_function(word)) {
                    if(strncmp(word, "main", 4) == 0) {
                        fwrite(word, 1, strlen(word), writeFile);
                        continue;
                    }
                    if(word[strlen(word) - 1] == ';') {
                        word[strlen(word) - 1] = '\0';
                        hasInter = true;
                    }
                    strcpy(variables[top], word);
                    random_length_string(word, 16);
                    strcpy(replacement[top], word);
                    strcpy(&replacement[top][strlen(replacement[top])], strrchr(variables[top], '(' ));
                    fwrite(replacement[top], 1, strlen(replacement[top]), writeFile);
                    if(hasInter) {
                        fwrite(";",1, 1, writeFile);
                    }
                    ++top;
                }
            } 
            if(mode >= 2) {
                if(word[strlen(word) - 1] == ';') {
                    word[strlen(word) - 1] = '\0';
                    hasInter = true;
                }
                if(!is_function(word)) {
                    strcpy(variables[top], word);
                    random_length_string(word, 16);
                    strcpy(replacement[top], word);
                    fwrite(replacement[top], 1, strlen(replacement[top]), writeFile);
                    if(hasInter) {
                        fwrite(";",1, 1, writeFile);
                    }
                    ++top;
                }else {
                    fwrite(word, 1, strlen(word), writeFile);
                }
            } else {
                fwrite(word, 1, strlen(word), writeFile);
            }
        }else if(index != -1) {
            fwrite(replacement[index], 1, strlen(replacement[index]), writeFile);
            if(hasInter) fwrite(";",1, 1, writeFile);
        }else {
            fwrite(word, 1, strlen(word), writeFile);
            if(hasInter) fwrite(";",1, 1, writeFile);
        }

    }

    fseek(readFile, 0, SEEK_SET);
}



int main(int argc, char *argv[]){
    fstream readFile, writeFile;
    srand(time(NULL));

    if(argc < 2) {
        printf("Wrong parameter.\n");
        return 0;
    }
    
    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printf("Use:\n ./hw0402 -l [options] -i [input file] -o [output file]\n");
        printf("Level explanation:\n");
        printf("1. Add redundant blanks and newlines.\n2. Change the variable's name to a length 16 random strings.\n  •Ex: int a; -> int boe1n292fsdfs2gs;\n3. Change the function’s name to a length 16 random strings exceptmainand standard functions.\n  •Ex: void f(); -> void ifsmf3b12hensfnl();\n4. For all integers, change the value to an equation with at least oneaddition and one multiplication.\n   •Ex: int a=5; -> int a=7+6/2;\n");
        printf("Help:\n");
        printf("./hw0402 -h or ./hw0402 --help\n");
    }else if (strcmp(argv[1], "-l") == 0) {
        
        int mode = strtol(argv[2], NULL, 10);
        if(argc != 7) {
            printf("Wrong parameter.\n");
            return 0;
        }
        if(strcmp(argv[3], "-i") == 0) {
            readFile = fopen(argv[4], "r");
            if(readFile == NULL) {
                printf("Read file not found.\n");
                return 0;
            }
        }else {
            printf("Wrong parameter.\n");
            return 0;
        }
        if(strcmp(argv[5], "-o") == 0) {
            writeFile  = fopen(argv[6], "w");
        }else {
            printf("Wrong parameter.\n");
            fclose(readFile);
            return 0;
        }
        obfuscation(readFile, writeFile, mode);
        fclose(readFile);
        fclose(writeFile);

    }
}