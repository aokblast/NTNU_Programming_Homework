#include "all.h"
#include "lrc.h"

#define DEBUG 0

void setting_the_fucking_color(lrcobj *obj, int status) {
    obj->iscolored = status;
}

void __lrc_header_handler(lrcobj *obj) {
    char line[1000];
    fgets(line, 1000, obj->src);
    size_t offset = ftell(obj->src);
    while(!feof(obj->src)) {
        char type[10], content[1000];
        if(line[0] == '\r') {
            fgets(line, 1000, obj->src);
            continue;
        }
        strcpy(type, strtok(line, "[:]"));
        strcpy(content, strtok(NULL, "[:]"));
        
        if(strcmp(type, "al") == 0) {
            strcpy(obj->header->album, content);
        }else if (strcmp(type, "ar") == 0) {
            strcpy(obj->header->artist, content);
        }else if (strcmp(type, "au") == 0){
            strcpy(obj->header->composer, content);
        }else if (strcmp(type, "by") == 0) {
            strcpy(obj->header->by, content);
        }else if (strcmp(type, "offset") == 0) {
            obj->header->offset = strtol(content, NULL, 10);
        }else if (strcmp(type, "re") == 0) {
            strcpy(obj->header->re, content);
        }else if (strcmp(type, "ti") == 0) {
            strcpy(obj->header->title, content);
        }else if (strcmp(type, "ve") == 0) {
            strcpy(obj->header->version, content);
        }else if(strcmp(type, "la") == 0){
            strcpy(obj->header->language, content);
        } else{
            break;
        }
        offset = ftell(obj->src);
        fgets(line, 1000, obj->src);
        
    }
    fseek(obj->src, offset, SEEK_SET);
}

lrcobj *read_lrc(const char *fileName) {
    if(strcmp(strrchr(fileName, '.'), ".lrc") != 0) {
        return NULL;
    }
    
    lrcobj *result = calloc(1, sizeof(lrcobj));
    
    if((result->src = fopen(fileName, "r")) == NULL) return NULL;
    memset(result->colors, 0, sizeof(result->colors));
    memset(result->people, 0, sizeof(result->people));
    memset(result->header, 0, sizeof(result->header[0]));
    
    __lrc_header_handler(result);

    result->line_no = 0;
    result->iscolored = 1;
    return result;
}

void lrc_info_display(lrcobj *lyric){
    lrcheader *header = lyric->header;
    if(header->album[0] != 0) {
        printf("Album: %s\n", header->album);
    }else {
        printf("Album: Unknow\n");
    }
    if(header->artist[0] != 0) {
        printf("Artist: %s\n", header->artist);
    }else {
        printf("Artist: Unknow\n");
    }
    if(header->composer[0] != 0) {
        printf("Composer: %s\n", header->composer);
    }else {
        printf("Composer: Unknow\n");
    }
    if(header->by[0] != 0) {
        printf("Creator: %s\n", header->by);
    }else {
        printf("Creator: Unknow\n");
    }
    printf("Offset: %d\n", header->offset);
    if(header->re[0] != 0) {
        printf("Player: %s\n", header->re);
    }else {
        printf("Player: Unknow\n");
    }
    if(header->title[0] != 0) {
        printf("Title: %s\n", header->title);
    }else {
        printf("Title: Unknow\n");
    }
    if(header->version[0] != 0){
        printf("Version: %s\n", header->version);
    }else{
        printf("Version: Unknow\n");
    }
    if(header->language[0] != 0) {
        printf("Language: %s\n", header->language);
    }else{
        printf("Language: Unknow\n");
    }
}

void printf_color(color c) {
    
    if(c == red ) printf(ANSI_COLOR_RED);
    else if (c == green) printf(ANSI_COLOR_GREEN);
    else if (c == yellow) printf(ANSI_COLOR_YELLOW);
    else if (c == blue) printf(ANSI_COLOR_BLUE);
    else if (c == magenta) printf(ANSI_COLOR_MAGENTA);
    else if (c == cyan) printf(ANSI_COLOR_CYAN);
    else if (c == white) printf(ANSI_COLOR_WHITE);
}


void play_lrc(lrcobj *lyric){
    char line[1000];
    double last_time = 0;
    while(fgets(line, 1000, lyric->src) != NULL) {
        if(DEBUG)printf("Line: %s\n", line);
        if(line[0] == '\r') {
            continue;
        }
        else if(line[0] != '[' && line[strlen(line) - 3] == ':') {
            if(lyric->iscolored == 0) continue;
            int i = 0, isin = 0;
            strtok(line, ":");
            for(; i < 8; ++i) {
                if(strcmp(line, lyric->people[i]) == 0){
                    
                    printf_color(lyric->colors[i]);
                    isin = 1;
                    break;
                }else if(lyric->people[i][0] == 0) break;
            }
            if(isin) continue;
            strcpy(lyric->people[i], line);
            lyric->colors[i] = i + 1; 
            printf_color(lyric->colors[i]);
            continue;
        }else if(line[0] == '['){
            int minutes = 0;
            double sec = 0;
            sscanf(line, "[%d: %lf]", &minutes, &sec);
            double time = minutes * 60 + sec;   

            struct timespec req = {(int)((time - last_time)),(int)(((time - last_time) - (int)(time - last_time))*1000000000)};
            nanosleep(&req, NULL);
            char *token = strtok(line ,"]");
            token = strtok(NULL, "]");
            puts(token);
            last_time = time;
        }else {
            puts(line);
        }
    }
    fseek(lyric->src, 0, SEEK_SET);


}
