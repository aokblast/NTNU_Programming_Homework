#ifndef BMPHANDLER
#define BMPHANDLER
#include "all.h"

typedef struct _sBmpHeader
{
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    uint32_t	width;
    uint32_t	height;
    uint16_t	planes;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((__packed__)) bmpHeader;

typedef struct _pixel24{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((__packed__)) pixel24;

typedef struct _pixel32{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} __attribute__ ((__packed__)) pixel32;

#endif