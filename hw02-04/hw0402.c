#include "all.h"

bool start_func = false;
bool end_func  = false;
bool is_end = false;

char opers[] = "+!~*/&%<>=^|?:'";

char *random_length_string(char *str, size_t length) {
    int i = 0;
    for(i = 0; i < length; ++i) {
        str[i] = rand() % 26 + 'a';
    }
    str[i] = '\0';
    return str;
}

int in_variables_index(char str[], char variables[][100]) {
    for(int i = 0; i < 10000; ++i) {
        if(strcmp(str, variables[i]) == 0) {
            
            return i;
        }else if (variables[i][0] == '\0') {
            return -1;
        }
    }
    return -1;
}

bool is_function(char str[]) {
    return (strrchr(str, '(') != NULL && strrchr(str, ')') != NULL);
}

void get_line(char *str, fstream readFile) {
    int index = 0;
    
    while(!feof(readFile)) {
        int c = fgetc(readFile);
        if(c == -1) {
            str[index++] = '\0';
            return;
        }
        if(c != ';' && c != '{' && c != '}') {
            if(strchr(opers, c) != NULL) {
                str[index++] = ' ';
                str[index++] = c;
                while(strchr(opers, (c = fgetc(readFile)) ) != NULL) {
                    str[index++] = c;
                }
                str[index++] = ' ';
            }else if(c == '(') {
                while(str[--index] == ' ');
                ++index;
                str[index++] = '(';
                str[index++] = ' ';
                continue;
            }else if (c == ')') {
                str[index++] = ' ';
            }else if( c == ',') {
                str[index++] = ' ';
                str[index++] = ',';
                str[index++] = ' ';
                continue;
            }
            if(c == '\n') c = ' ';
            str[index++] = c;
        }else {
            if(c == ';') is_end = true;
            else if(c == '{') start_func = true;
            else if(c == '}') end_func = true;
            str[index++] = '\0';
            return;
        }
    }
}

void add_redundant(fstream writeFile) {
    int times = rand() % 10;
    while(times--) fwrite((rand() % 2 == 0) ? "\n" : " ", 1, 1, writeFile);
}

char *operator_obfs(char *str, int a) {
     
}


void obfuscation(fstream readFile, fstream writeFile, int mode) {
    char variables[10000][100] = {0}, replacement[10000][100] = {0};
    bool obfus_func = (mode >= 3 ? true : false);
    bool obfus_variable = (mode >= 2 ? true : false);
    bool obfus_redun = (mode >= 1 ? true : false);
    bool obfus_oper = (mode >= 4 ? true : false);
    string line;
    int top = 0;
    //skip pre-processor
    while(!feof(readFile)) {
        uint64_t lastPos = ftell(readFile);
        fgets(line, 10005, readFile);
        if(line[0] != '#' && line[0] != '\n') {
            fseek(readFile, lastPos, SEEK_SET);
            break;
        }else {
            fwrite(line, 1, strlen(line), writeFile);
        }
    }
    string word , tmp;
    //obfuscation
    while(!feof(readFile)) {
        get_line(line, readFile);
        bool isFunction = is_function(line);
        if(line[0] == '\n') continue;
        char *iter = strtok(line, " ");
        if(iter == NULL) goto end;
        if(strcmp(iter, "char") == 0 || strcmp(iter, "int") == 0) {
            if(obfus_redun)add_redundant(writeFile);
            fwrite(iter, 1, strlen(iter), writeFile);
            iter = strtok(NULL, " ");
            if(strcmp(iter, "main(") == 0) {
                fwrite(" ", 1, 1, writeFile);
                fwrite(iter, 1, strlen(iter), writeFile);
                fwrite(")", 1, 1, writeFile);
                goto end;
            }
            if(obfus_redun)add_redundant(writeFile);
            if((isFunction && obfus_func) || (!isFunction && obfus_variable)) {
                strcpy(replacement[top], random_length_string(tmp, 16));
                if(isFunction)strncpy(variables[top], iter, strchr(iter, '(') - iter);
                else strcpy(variables[top], iter);
                fwrite(replacement[top], 1, strlen(replacement[top]), writeFile);
                if(isFunction) fwrite(strchr(iter, '('), 1, strlen(strchr(iter, '(')), writeFile);
                while((iter = strtok(NULL, " ")) != NULL) {
                    int index = in_variables_index(iter, variables);
                    if(obfus_redun)add_redundant(writeFile);
                    if(index != -1) {
                        fwrite(replacement[index], 1, strlen(replacement[index]), writeFile);
                    }else {
                        fwrite(iter, 1, strlen(iter), writeFile);
                    }
                }
                 ++top;
            }else {
                if(obfus_redun)add_redundant(writeFile);
                fwrite(iter, 1, strlen(iter), writeFile);
                while((iter = strtok(NULL, " ")) != NULL) {
                    if(obfus_redun)add_redundant(writeFile);
                   fwrite(iter, 1, strlen(iter), writeFile); 
                }
            }
           
            
        }else {
            bool left = false;
            do {
                int index;
                if(strchr(iter, '(') != NULL) {
                    left = true;
                    *(strchr(iter, '(')) = '\0';
                }
                
                index = in_variables_index(iter, variables);
                if(obfus_redun)add_redundant(writeFile);
                if(index != -1) {
                    fwrite(replacement[index], 1, strlen(replacement[index]), writeFile);
                }else {
                    fwrite(iter, 1, strlen(iter), writeFile);
                }
                if(left) fwrite("(", 1, 1, writeFile), left = false;
            }while((iter = strtok(NULL, " ")) != NULL);
            

        }
        
        if(obfus_redun)add_redundant(writeFile);
        end:
        if(is_end)fwrite(";", 1, 1, writeFile), is_end = false;
        else if(start_func) fwrite("{", 1, 1, writeFile), start_func = false;
        else if(end_func) fwrite("}", 1, 1, writeFile), end_func = false;
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