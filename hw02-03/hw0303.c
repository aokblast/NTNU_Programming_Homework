#include "all.h"
#include "bmp.h"

int main(){
    char fileName[100];
    double angle = 0;

    printf("Please input a BMP file:");
    scanf(" %s", fileName);
    FILE *readFile = fopen(fileName, "r");

    bmpHeader header[1];
    fread(header, sizeof(bmpHeader), 1, readFile);

    printf("Please input the output BMP file name:");
    scanf(" %s", fileName);
    FILE *writeFile = fopen(fileName, "w");

    printf("Please input the angle:");
    scanf(" %lf", &angle);

    if(angle <= 0 || angle >= 90) {
        printf("Iligal angle.\n");
        fclose(readFile);
        fclose(writeFile);        
        return 0;   
    }

    uint32_t origWidth = header->width;
    header->width += (header->height)*tan((90 - angle) / 180 * 3.1415926535);
    if(header->width % 4 != 0) header->width += (4 - (header->width % 4));
    fwrite(header, sizeof(bmpHeader), 1, writeFile);

    for(int i = 0; i < header->height; ++i) {
        int offset = (i) * tan((90 - angle) / 180 * 3.1415926535);
        
        pixel24 new_row[header->width], old_row[origWidth];
        pixel24 white = {255, 255, 255};
        fread(old_row, sizeof(pixel24), origWidth, readFile);
        for(int j = 0; j < header->width; ++j) {
            new_row[j] = white;
        }
        for(int j = 0; j < origWidth; ++j) {
            new_row[j + offset] = old_row[j];
        }

        fwrite(new_row, sizeof(pixel24), header->width, writeFile);
    }
    fclose(readFile);
    fclose(writeFile);
    

}