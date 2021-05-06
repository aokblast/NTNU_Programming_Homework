#include "myprintf.h"
#include "all.h"


char hexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void reverseStr(char *str){
    int len = strlen(str);
    for(int i = 0; i < len / 2; ++i) {
        char tmp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = tmp;
    }
}

char *inttohex(int num){
    char *result = calloc(20, sizeof(char));
    int i = 0;
    uint32_t tmp = (uint32_t) num;
    while(i < 8) {
        result[i++] = hexTable[(tmp & 0xF)];
        tmp >>= 4;
    }
    reverseStr(result);
    return result;
}

char *inttostr(int num){
    bool neg = (num > 0 ? false : true);
    num = abs(num);
    char *result = calloc(20, sizeof(char));
    int i = 0;
    while(num > 0) {
        result[i++] = hexTable[num % 10];
        num /= 10;
    }
    result[i++] = (neg ? '-' : '+');
    reverseStr(result);
    return result;
}



int myprintf(char *format, ... ) {
    va_list args;
    va_start(args, format);
    int count = 0;
    char *iter = format;
    for(; *iter != '\0'; ++iter) {
        bool space = false, padding = false, left = false, hex = false, sign = false; 
        int min_width = 0;
        if((*iter) == '%') {
            ++iter;
            while(strchr(" +-#0", *iter) != NULL)  {    
                switch(*iter) {
                    case ' ':
                        space = true;
                        break;
                    case '+':
                        sign = true;
                        break;
                    case '-':
                        left = true;
                        break;
                    case '#':
                        hex = true;
                        break;
                    case '0':
                        padding = true;
                        break;
                }
                ++iter;
                
            }
            
            while(strchr("sXxid", *iter) == NULL ) {
                min_width = min_width * 10 + (*(iter++)) - '0';
            }
            switch(*iter) {
                case 's': {
                    char *arg = va_arg(args, char *);
                    int totalWhite = min_width - strlen(arg);
                    if(min_width) {
                        if(left) {
                            while((*arg) != '\0') fputc(*(arg++), stdout), ++count;
                            while(totalWhite > 0) fputc(' ', stdout), ++count, --totalWhite;
                        }else {
                            while(totalWhite > 0) fputc(' ', stdout), ++count, --totalWhite;
                            while((*arg) != '\0') fputc(*(arg++), stdout), ++count;
                        }
                    }else{
                        while((*arg) != '\0') fputc(*(arg++), stdout), ++count; 
                    }
                }
                break;
                case 'X': 
                case 'x': {
                    int arg = va_arg(args, int);
                    char *hexStr = inttohex(arg);
                    int totalLen = strlen(hexStr) + (hex ? 2 : 0);
                    if(left) {
                        if(hex) {
                            fputc('0', stdout), ++count;
                            fputc(*iter, stdout), ++count;
                        }
                        for(int i = 0; hexStr[i] != '\0'; ++i) fputc(((*iter) == 'x' ? tolower(hexStr[i]) : hexStr[i]), stdout), ++count;
                        while(min_width - totalLen > 0) fputc((padding ? '0' : ' '), stdout), ++count, --min_width;
                    }else {
                        while(min_width - totalLen > 0) fputc((padding ? '0' : ' '), stdout), ++count, --min_width;
                        if(hex) {
                            fputc('0', stdout), ++count;
                            fputc(*iter, stdout), ++count;
                        }
                        for(int i = 0; hexStr[i] != '\0'; ++i) fputc(((*iter) == 'x' ? tolower(hexStr[i]) : hexStr[i]), stdout), ++count;
                    }
                    free(hexStr);
                }
                break;
                case 'i': 
                case 'd': {
                    int arg = va_arg(args, int);
                    char *intStr = inttostr(arg);
                    int totalLen =  strlen(intStr) - (arg < 0 ? 0 : sign ? 0 : space ? 0 : 1);
                    if(left) {
                        if(space || sign || intStr[0] == '-') fputc((arg < 0 ? intStr[0] : sign ? intStr[0] : space ? ' ' : '\0'), stdout), ++count;
                        for(int i = 1; intStr[i] != '\0'; ++i) fputc(intStr[i], stdout), ++count;
                        while(min_width > totalLen) fputc(' ', stdout), ++count, --min_width;
                        
                    }else{
                        if(padding) {
                            if(space || sign || intStr[0] == '-') fputc((arg < 0 ? intStr[0] : sign ? intStr[0] : space ? ' ' : '\0'), stdout), ++count;
                            while(min_width > totalLen) fputc('0', stdout), ++count, --min_width;
                            for(int i = 1; intStr[i] != '\0'; ++i) fputc(intStr[i], stdout), ++count;  
                        }else{
                            while(min_width > totalLen) fputc(' ', stdout), ++count, --min_width;
                            if(space || sign || intStr[0] == '-') fputc((arg < 0 ? intStr[0] : sign ? intStr[0] : space ? ' ' : '\0'), stdout), ++count;
                            for(int i = 1; intStr[i] != '\0'; ++i) fputc(intStr[i], stdout), ++count;                    
                        }
                    }
                    

                    free(intStr);
                }
                break;
            }
        }
        else {
            fputc(*iter, stdout);
            ++count;
        }
        
    }
    return count;
}