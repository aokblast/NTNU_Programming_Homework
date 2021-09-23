#include "all.h"
#include "linux_header.h"

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

const struct option long_option[] = {
    {"output", required_argument, 0, 'o'},
    {"input", required_argument, 0, 'i'},
    {"help", optional_argument, 0, 'h'},
    {"angle", required_argument, 0, 'a'},
    {0, 0, 0, 0}
};


int main(int argc, char *argv[]){
    char c;
    int angle = 0;
    char *errorPtr;
    string inputFileName, outputFileName;
    while((c = getopt_long(argc, argv, "a:i:o:h", long_option, NULL)) != EOF)
    switch(c){
                case 'o':
                    strcpy(outputFileName, optarg);
                    break;
                case 'i':
                    strcpy(inputFileName, optarg);
                    break;
                case 'h':
                    printf("fin02:\n  -a, --angle: angle for clockwise rotation\n  -i, --input: input file\n  -o, --output: output file\n  -h, --help: This description");
                    break;
                case 'a':
                    angle = strtol(optarg, &errorPtr, 10);
                    if(*errorPtr != '\0') printf("Wrong input.\n"); 
                    break;    
        
    }
    
    
    FILE *readfile = fopen(inputFileName, "r");
    if(readfile == NULL) printf("Wrong input file.\n");
    FILE *outputFile = fopen(outputFileName, "w");
    
    //read header
    
    bmpHeader header[1];
    
    fread(header, sizeof(header), 1, readfile);
    if(strncmp(header->bm, "BM", 2)) {
        printf("Wrong input file.\n");
        return 0;
    }
    pixel24 origin[header->height][header->width];
    fread(origin, sizeof(pixel24), header->height * header->width, readfile);
    bmpHeader newHeader[1] = {*header};
    if(angle <= 270) angle = -angle;
    //get new height and width
    newHeader->height = (cos(angle / 180.0) * header->width - sin(angle / 180.0) * header->height);
    newHeader->height = abs(newHeader->height);
    newHeader->width = (-sin(angle / 180.0) * header->width + cos(angle / 180.0) * header->height);
    newHeader->width = abs(newHeader->width);
    angle = -abs(angle);
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
