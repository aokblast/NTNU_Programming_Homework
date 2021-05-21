#include "all.h"
#include "linux_header.h"
#define ENCODE 1
#define DECODE 2

static const unsigned char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char intTable[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};


const struct option long_option[] = {
    {"output", required_argument, 0, 'o'},
    {"enc", required_argument, 0, 'e'},
    {"dec", required_argument, 0, 'd'},
    {0, 0, 0, 0}
};

void to_base64(uint8_t to[4], uint8_t from[3]) {
    to[0] = from[0] >> 2;
    to[1] = (((from[0] & 0x03) << 4) | (from[1] >> 4));
    to[2] = (((from[1] & 0x0F) << 2) | (from[2] >> 6));
    to[3] = (from[2] & 0x3F);
    to[0] = base64Table[to[0]];
    to[1] = base64Table[to[1]];
    to[2] = base64Table[to[2]];
    to[3] = base64Table[to[3]];
}

void to_int(uint8_t to[3], uint8_t from[4]) {
    from[0] = intTable[from[0]];
    from[1] = intTable[from[1]];
    from[2] = intTable[from[2]];
    from[3] = intTable[from[3]];
    to[0] = (from[0] << 2 ) | ((from[1] & 0x30) >> 4);
    to[1] = ((from[1] & 0x0F) << 4) | ((from[2] & 0x3C) >> 2);
    to[2] = ((from[2] & 0x03) << 6) | (from[3]);
}

inline static void print_base64(uint8_t b64[]) {
    printf("%c%c%c%c", b64[0], b64[1], b64[2], b64[3]);
}

int main (int argc, char *argv[]) {
    int c = 0, flag = 0;
    uint8_t  intArr[3] = {0}, base64[4] = {0};
    fstream outputFile, inputFile;
    while((c = getopt_long(argc, argv, "o:e:d:", long_option, NULL)) != EOF) {
        switch(c){
            case 'o': {
                outputFile = fopen(optarg, "w");
                break;
            }
            
            case 'e': {
                if((inputFile = fopen(optarg, "r")) == NULL) {
                    goto error_file;
                }
                flag = ENCODE;
                break;
            }
            
            case 'd': {
                if((inputFile = fopen(optarg, "r")) == NULL) {
                    goto error_file;
                }
                flag = DECODE;
                break;
            }
            
        }
    }
    if(flag == DECODE) {
        while(!feof(inputFile)) {
            int size = fread(base64, 1, 4, inputFile);
            to_int(intArr, base64);
            fwrite(intArr, 1, 3, outputFile);
        }
        
    }else if(flag == ENCODE) {
        while(!feof(inputFile)) {
            int size = fread(intArr, 1, 3, inputFile);
            to_base64(base64, intArr);
            if(size < 3) for(int i = 3; i > size; --i) base64[i] = '=';
            fwrite(base64, 1, 4, outputFile);
        }   
    }else {
        printf("Wrong parameter.\n");
        return 0;
    }
    
    return 0;

    error_file:

    printf("Wrong file.\n");

    return 0;

}