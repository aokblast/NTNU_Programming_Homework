#ifndef LRCPARSER
#define LRCPARSER

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[0m"

#include "all.h"
#define MAXLRCSTRLEN 50

typedef size_t line_size;

typedef enum _color{
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white
} color;

typedef struct _lrcheader{
    char album[MAXLRCSTRLEN];
    char artist[MAXLRCSTRLEN];
    char composer[MAXLRCSTRLEN];
    char by[MAXLRCSTRLEN];
    int offset;
    char re[MAXLRCSTRLEN];
    char title[MAXLRCSTRLEN];
    char version[MAXLRCSTRLEN];
    char language[MAXLRCSTRLEN];
} lrcheader;

typedef struct _lrcobject{
    FILE *src;
    line_size line_no;
    char people[8][MAXLRCSTRLEN];
    color colors[8];
    lrcheader header[1];
    char iscolored;
} lrcobj;

lrcobj *read_lrc(const char *fileName);
void play_lrc(lrcobj *lyric);
void lrc_info_display(lrcobj *lyric);
void setting_the_fucking_color(lrcobj *obj, int status);
#endif