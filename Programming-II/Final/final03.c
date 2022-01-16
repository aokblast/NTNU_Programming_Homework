#include "all.h"
#include "linux_header.h"
#include <curl/curl.h>


size_t writeStr(void *ptr, size_t size, size_t nmemb, char *str) {
    memcpy(&str[strlen(str)], ptr,  size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[]){
    char *webcontent = calloc(100000000, 1);
    string url = "https://simple.wiktionary.org/wiki/";
    CURL *content = curl_easy_init(); 
    strcpy(&url[strlen(url)], argv[2]);
    curl_easy_setopt(content, CURLOPT_URL, url );
    curl_easy_setopt(content, CURLOPT_WRITEFUNCTION, writeStr);
    curl_easy_setopt(content, CURLOPT_WRITEDATA, webcontent );
    curl_easy_perform(content);
    curl_easy_cleanup(content);
    char *iter = webcontent, *prev;
    //printf("%s\n", iter);
    iter = strstr(iter, "mw-parser-output");
    iter = iter + 18;
    iter = strstr(iter, "<h2>");
    iter = iter + 5;
    prev = iter;
    //printf("%s", iter);
    while((iter = strstr(iter, "<h2>")) != NULL){
        iter += 5;
        *iter = '\0';
        ++iter;
        prev = strchr(prev, '>');
        ++prev;
        while(*prev != '<'){
            fputc(*prev, stdout);
            ++prev;
        }
        printf(": ");
        prev = strstr(prev, "<tbody>");
        prev += 8;
        char *table = prev;
        prev = strstr(prev, "</tbody>");
        prev += 9;
        *prev = '\0';
        ++prev;
        table = strstr(table, "<b>");
        table = strchr(table, 'a');
        table = strchr(table, '>');
        ++table;
        while(*table != '<'){
            fputc(*table, stdout);
            ++table;
        }
        while((table = strstr(table, "<b>")) != NULL) {
            fputc('/', stdout);
            table = strchr(table, 'a');
            table = strchr(table, '>');
            ++table;
            while(*table != '<'){
                fputc(*table, stdout);
                ++table;
            }
        }
        printf("\n");
        prev = strstr(prev, "<ol>");
        prev += 4;
        char *sentence = prev;
        prev = strstr(prev, "</ol>");
        *prev = '\0';
        int level = 0;
        
        /*
        while( *sentence != '\0'){
            
            if(*sentence == '<'){
                ++level;
            }else if(*sentence == '>'){
                --level;
            }else if(level == 0 && *sentence != '\n'){
                fputc(*sentence, stdout);
                if(*sentence == '.'){
                    fputc('\n', stdout);
                }
            }
            
            ++sentence;
            
        }
        */

        printf("\n");

        
        prev = iter;
    }

    free(webcontent);
    

}