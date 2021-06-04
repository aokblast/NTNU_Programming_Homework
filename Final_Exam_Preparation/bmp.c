#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct _sBmpHeader
{
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    int32_t	width;
    int32_t	height;
    uint16_t	planes;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((__packed__)) bmpHeader;

typedef struct _Pixel24{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((__packed__)) pixel24;



int main(){
    char fileName[10000];
    printf("Please enter the file name.\n");
    scanf(" %s", fileName);
    int angle = 0;
    printf("Rotation angle (int, 0-360): ");
    scanf(" %d", &angle);
    FILE *readfile = fopen(fileName, "r");
    FILE *outputFile = fopen("output.bmp", "w");
    //read header
    bmpHeader header[1];
    fread(header, sizeof(header), 1, readfile);
    pixel24 origin[header->height][header->width];
    fread(origin, sizeof(pixel24), header->height * header->width, readfile);
    bmpHeader newHeader[1] = {*header};
    //get new height and width
    newHeader->height = (sin(angle / 180.0) * header->width + cos(angle / 180.0) * header->height);
    newHeader->height = abs(newHeader->height);
    newHeader->width = (cos(angle / 180.0) * header->width + sin(angle / 180.0) * header->height);
    newHeader->width = abs(newHeader->width);
    //bmp padding
    if(newHeader->width % 4 != 0) newHeader->width += (4 - newHeader->width % 4);
    
    fwrite(newHeader, sizeof(header), 1, outputFile);
    pixel24 new[newHeader->height][newHeader->width];
    pixel24 white = {255, 255, 255};
    for(int i = 0; i < newHeader->height; ++i){
        for(int j = 0; j < newHeader->width; ++j){
            //use inverse rotation matrix
            int x_loc = (cos(angle / 180.0) * (j - newHeader->width / 2) + sin(angle / 180.0) * (i - newHeader->height / 2)) ;
            int y_loc = (-sin(angle / 180.0) * (j - newHeader->width / 2) + cos(angle / 180.0) * (i - newHeader->height / 2)) ;
            if(-header->width / 2 < x_loc && x_loc < header->width / 2 && -header->width / 2 < y_loc && y_loc < header->height / 2)new[i][j] = origin[y_loc + header->height / 2][x_loc + header->width / 2];
            else new[i][j] = white;
            
        }
    }
    fwrite(new, sizeof(pixel24), newHeader->width * newHeader->height, outputFile);
    fclose(readfile);
    fclose(outputFile);
}
