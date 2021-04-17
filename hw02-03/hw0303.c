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

typedef struct _pixel{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((__packed__)) pixel;

int main(){
    char fileName[100];
    double angle = 0;
    printf("Please input a BMP:");
    scanf(" %s", fileName);
    FILE *readFile = fopen(fileName, "r");
    bmpHeader header[1];
    fread(header, sizeof(bmpHeader), 1, readFile);
    printf("Please input the output BMP file name:");
    scanf(" %s", fileName);
    FILE *writeFile = fopen(fileName, "w");
    printf("Please input the angle:");
    scanf(" %lf", &angle);
    uint32_t origWidth = header->width;
    header->width += (header->height)*sin(angle / 180);
    if(header->width % 4 != 0) header->width += (4 - (header->width % 4));
    fwrite(header, sizeof(bmpHeader), 1, writeFile);
    for(int i = 0; i < header->height; ++i) {
        int offset = (i) * sin(angle / 180);
        pixel new_row[header->width], old_row[origWidth];
        pixel white = {255, 255, 255};
        fread(old_row, sizeof(pixel), origWidth, readFile);
        for(int j = 0; j < header->width; ++j) {
            new_row[j] = white;
        }
        for(int j = 0; j < origWidth; ++j) {
            new_row[j + offset] = old_row[j];
        }
        fwrite(new_row, sizeof(pixel), header->width, writeFile);
    }
    fclose(readFile);
    fclose(writeFile);
    

}