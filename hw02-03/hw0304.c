#include "all.h"
#include "bmp.h"

int main(){
    char fileName[100];
    int alpha;
    printf("Please input a BMP file:");
    scanf(" %s", fileName);
    FILE *readFile = fopen(fileName, "r");
    printf("Please input the output BMP file name:");
    scanf(" %s", fileName);
    FILE *writeFile = fopen(fileName, "w");
    bmpHeader header[1];
    printf("Alpha(0-31):");
    scanf(" %d", &alpha);
    fread(header, sizeof(bmpHeader), 1, readFile);
    if(header->bpp != 24) {
        printf("Wrong bpp.\n");
        return 0;
    }
    header->bpp = 32;
    header->bitmap_size = 0;
    header->compression = 6;
    header->header_size = 56;
    header->offset = 118;
    fwrite(header, sizeof(bmpHeader), 1, writeFile);
    uint32_t masks[4] = {0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};
    fwrite(masks, sizeof(uint32_t), sizeof(masks), writeFile);

    while(!feof(readFile)){
        pixel32 pix={0,0,0,0};
        fread(&pix, sizeof(pixel24), 1, readFile);
        pix.a = alpha * 8;
        fwrite(&pix, sizeof(pixel32), 1, writeFile);
    }
    fclose(readFile);
    fclose(writeFile);
}