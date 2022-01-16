#ifndef SAN5PKEDITOR
#define SAN5PKEDITOR

#include "all.h"

#define CONEWALK 1 << 1
#define HOOKWALK 1 << 2
#define DUSTPANWALK 1 << 3
#define CRANEWING 1 << 4
#define FISHSCALE 1 << 5
#define KOGETSU 1 << 6
#define SHARPARROW 1 << 7
#define BALANCE 1 << 8
#define LONGSNAKE 1 << 9
#define SQUCIR 1 << 10
#define KARIKOWALK 1 << 11
#define WATERBATTLE 1 << 12



typedef struct _monarch_info {
    uint16_t heart;
    uint16_t unknow_1;
    uint32_t gold;
    uint32_t food;
}__attribute__ ((__packed__)) monarInfo;

typedef struct _city_info {
    uint32_t prepare_soldier;
    uint64_t unknow_1;
    uint16_t population;
    uint16_t defeat;
    uint16_t unknow_2;
    uint16_t development;
    uint16_t business;
    uint16_t unknow_3;
    uint8_t unknow_4;
    uint8_t water;
    uint8_t loyalty;
    uint8_t moral;
    uint8_t train;
}__attribute__ ((__packed__)) cityinfo;

typedef struct _hero_info {
    uint16_t reputation;
    uint16_t exp;
    uint16_t soldier;
    uint16_t unknow_1;
    uint8_t force;
    uint8_t wise;
    uint8_t policy;
    uint8_t charm;
    uint8_t train;
    uint8_t moral;
    uint8_t unknow_2;
    uint8_t loyalty;
    uint32_t unknow_3;
    uint16_t unknow_4;
    uint8_t unknow_5;
    uint32_t skills;
}__attribute__ ((__packed__)) heroinfo;

void edit_monarch(fstream readFile);
void edit_city(fstream readFile);
void edit_commander(fstream readFile);


#endif