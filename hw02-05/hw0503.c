#include "all.h"
#include "linux_header.h"
#include <curl/curl.h>

char *webpage;

typedef struct _articleQuery {
    char *title;
    char *authors;
    char *source;
    char *year;
} articleQuery;


size_t writeStr(void *ptr, size_t size, size_t nmemb, char *str) {
    memcpy(&str[strlen(str)], ptr,  size * nmemb);
    return size * nmemb;
}

void get_Author(char *content, char **authors) {
    (*authors) = calloc(1000, 1);
    size_t length = 0;
    content = strstr(content, "\"authors\"");
    content = strstr(content, "\"text\"");

    content = strchr(content, ':');
    content = strchr(content, '\"') + 1;
    while(*content != '\"') {
        (*authors)[length++] = *(content++);
    }
    while((content = strstr(content, "\"text\"")) != NULL) {
        (*authors)[length++] = ',';
        (*authors)[length++] = ' ';
        content = strchr(content, ':');
        content = strchr(content, '\"') + 1;
        while(*content != '\"') {
            (*authors)[length++] = *(content++);
        }
    }
}

void get_Title(char *content, char **title) {
    (*title) = calloc(1000, 1);
    content = strstr(content, "\"title\"");
    content = strchr(content, ':');
    content = strchr(content, '\"') + 1;
    size_t length = 0;
    while(*content != '\"') {
        (*title)[length++] = *(content++);
    }
    
}

void get_Source(char *content, char **source) {
    (*source) = calloc(1000, 1);
    content = strstr(content, "\"venue\"");
    content = strchr(content, ':');
    content = strchr(content, '\"') + 1;
    size_t length = 0;
    while(*content != '\"') {
        (*source)[length++] = *(content++);
    }    
}

void get_Year(char *content, char **year) {
    (*year) = calloc(1000, 1);
    content = strstr(content, "\"year\"");
    content = strchr(content, ':');
    content = strchr(content, '\"') + 1;
    size_t length = 0;
    while(*content != '\"') {
        (*year)[length++] = *(content++);
    }
}

void free_artucleQuery(articleQuery *query, size_t total){
    for(int i = 0; i < total; ++i) {
        free(query[i].authors);
        free(query[i].source);
        free(query[i].title);
        free(query[i].year);
    }
    free(query);
}

size_t dblp_Query_Parsing(char *webcontect, articleQuery **result){
    char *iter = strstr(webcontect, "hits");
    if(*iter == '\0') {
        (*result) = NULL;
        return 0;
    }
    char *info = strstr(iter + 4, "\"hit\"");
    (*result) = calloc(10, sizeof(articleQuery));
    size_t totalNum = 0;
    info = strstr(info, "\"info\"");
    while(info != NULL) {
        char *next = strstr(info+5, "\"info\"");
        if(next != NULL) *(next - 1) = '\0';
        get_Author(info, &(*result)[totalNum].authors);
        get_Title(info, &(*result)[totalNum].title);
        get_Source(info, &(*result)[totalNum].source);
        get_Year(info, &(*result)[totalNum].year);
        ++totalNum;
        info = next;
    }
    return totalNum;
} 

int main(int argc, char *argv[]){
    int c = 0;
    string url = "https://dblp.org/search/publ/api?format=json&h=10&q=";
    CURL *content = curl_easy_init();
    webpage = calloc(100000000, 1);
    while((c = getopt(argc, argv, "q:")) != EOF){
        switch(c) {
            case 'q': {
                strcpy(&url[strlen(url)], optarg);
                curl_easy_setopt(content, CURLOPT_URL, url );
                curl_easy_setopt(content, CURLOPT_WRITEFUNCTION, writeStr);
                curl_easy_setopt(content, CURLOPT_WRITEDATA, webpage );
                curl_easy_perform(content);
                curl_easy_cleanup(content);
            }
            break;
        }
        
    }
    articleQuery* result;
    size_t totalRes = dblp_Query_Parsing(webpage, &result);

    if(totalRes == 0) {
        printf("No result.\n");
        return 0;
    }

    for(int i = 0; i < totalRes; ++i) {
        printf("Paper %02d\n", i + 1);
        printf("Title: %s\n", result[i].title);
        printf("Author: %s\n", result[i].authors);
        printf("Source: %s\n", result[i].source);
        printf("Year: %s\n", result[i].year);
    }

    free(webpage);
    free_artucleQuery(result, totalRes);
}