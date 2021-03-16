#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char result[2048] = {0};

void removeZero(char Str[]) {
    char *iter = Str;
    for(; *iter != '\n'; ++iter);
    *iter = '\0';
}

bool jsonQuery(const char jsonStr[], char query[]) {
    char *token = strtok(query, ".");
    const char *iter = jsonStr;
    int layer = 0, presentLayer = 0, type = 0;
    char *arr = NULL;
    do{
        arr = strchr(token,'[');
        ++presentLayer;
        for(;*iter != '\0'; ++iter) {
            if(*iter == '{') ++layer;
            else if (*iter == ',') type = 0;
            else if (*iter == ':') type = 1;
            else if (*iter == '}') --layer;
            else if (*iter == '\"' && presentLayer == layer && type == 0) {
                ++iter;
                int i, j;
                for(i =0, j = 0; token[j] != 0 && &token[j] != arr;++i, ++j) {
                    if(iter[i] == '\\') ++i;
                    if(iter[i] != token[j]) break;
                }
                if((token[j] == '\0' || token[j] == '[') && iter[i] == '\"' && (iter[i + 1] == ' ' || iter[i + 1] == ':')) break;
                iter += i;
            }
        }
        if(*iter != '\0') {
            type = 0;
            iter = strchr(iter, ':');
            ++iter;
        }
        if(arr != NULL && *iter != '\0') {
            
            while(arr != NULL && *iter != '\0') {
                
                size_t index = 0;
                iter = strchr(iter, '[');
                ++iter;
                for(size_t i = 1; arr[i] != ']'; ++i ) {
                    index = index * 10 + arr[i] - '0';
                }
                for(int i = 0;index != i ;++iter) {
                    if(*iter == '{') ++layer;
                    else if(*iter == '}') --layer;
                    else if(layer == presentLayer && *iter == ',') ++i;
                }
                arr = strchr(arr, ']');
                arr = strchr(arr, '[');
            }
        }
    }while((token = strtok(NULL, ".")) != NULL && *iter != '\0');
    if(*iter == '\0') {
        return false;
    }
    
    else{
        int type = 0;
        while(*iter == ' ' || *iter == '\"') {
            if(*iter == '\"') type = 1;
            ++iter;
        }
        int i = 0;
        if(type == 1) {
            for(; *iter != '\"'; ++iter, ++i) {
                result[i] = *iter;
            }
        }
        else if(type == 0) {
                for(; *iter != ' ' && *iter != '}'  && *iter != ']' && *iter != ','; ++iter, ++i) {
                result[i] = *iter;
            }
        }
        result[i] = '\0';
        return true;
    }
}


int main(){
    char jsonStr[2050] = {0};
    char query[2050] = {0};
    memset(query, 0, 2050);
    memset(jsonStr, 0, 2050);
    printf("Please enter the json string:\n");
    fgets(jsonStr, 2050, stdin);
    int mode = 0;
    while(1) {
        printf("Choice (0:Exit ,1:Get) : ");
        if(scanf(" %d",&mode) != 1){
            printf("Wrong input. Please retry.\n");
            continue;
        }
        setbuf(stdin, NULL);
        if(mode == 1) {
            printf("Key:");
            fgets(query, 2050, stdin);
            removeZero(query);
            if(jsonQuery(jsonStr, query)) printf("Value:%s\n", result);
            else printf("Not in the jsonFile.\n");
            
        }else if (mode == 0){
            printf("Bye\n");
            break;
        }else {
            printf("Wrong input. Please retry.\n");

        }
    }
}