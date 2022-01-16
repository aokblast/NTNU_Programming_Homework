#include "all.h"
#include "linux_header.h"

char hexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

typedef struct _splitHeader {
    char sf[2];
    size_t size;
    size_t label;
    size_t name_length;
}__attribute__ ((__packed__)) splitHeader;

size_t getFileSize(fstream file) {
    if(file == NULL) return 0;
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void reverseStr(char *str){
    int len = strlen(str);
    for(int i = 0; i < len / 2; ++i) {
        char tmp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = tmp;
    }
}

char *size_type_to_string(char *result,size_t num) {
    if(num == 0){
        result[0] == '0';
        return result;
    }
    for (size_t i = 0; num > 0; i++)
    {
        result[i] = hexTable[num % 10];
        num /= 10;
    }
    reverseStr(result);
    return result;
}

void splitFile(fstream file, const char *infileName, size_t perSize) {
    size_t fileSize = getFileSize(file);
    string fileName, sizeTypeStr;
    strcpy(fileName, infileName);
    size_t nameSize = strlen(fileName), label = 1;
    fileName[nameSize++] = '.';

    while(fileSize > 0) {
        //Write header
        size_t writeSize = (fileSize > perSize ? perSize : fileSize);
        strcpy(&fileName[nameSize], size_type_to_string(sizeTypeStr, label));
        fileSize -= writeSize;
        splitHeader header[1] = {{"SF", writeSize, label, nameSize - 1}};
        fstream writeFile = fopen(fileName, "w");
        fwrite(header, 1, sizeof(splitHeader), writeFile);
        fwrite(infileName, 1, nameSize - 1, writeFile);
        //Write content
        uint8_t content[writeSize];
        fread(content, 1, writeSize, file);
        fwrite(content, 1, writeSize, writeFile);
        ++label;
        fclose(writeFile);
    }
    fseek(file, 0, SEEK_SET);
    
}

void recoverFile(fstream target, char *filesName[], size_t totalFileNum) {
    fstream readFiles[totalFileNum];
    fstream tmp = fopen(filesName[0], "r");
    if(tmp == NULL) {
        printf("We cannot find one or many files.\n");
        return;
    }
    splitHeader header[1];
    fread(header, sizeof(splitHeader), 1, tmp);
    if(strncmp(header->sf, "SF", 2) != 0) {
        fclose(tmp);
        printf("One or many files are not the split files.\n");
        return;
    }
    string origFileName;
    fread(origFileName, 1, header->name_length, tmp);
    fseek(tmp, 0, SEEK_SET);
    fclose(tmp);
    //readFile and check error
    for(int i = 0; i < totalFileNum; ++i) {
        tmp = fopen(filesName[i], "r");
        string tmpFileName;
        fread(header, sizeof(splitHeader), 1, tmp);
        fread(tmpFileName, 1, header->name_length, tmp);
        if(strncmp(header->sf, "SF", 2) != 0 || strcmp(tmpFileName, origFileName) != 0) {
            // free all opened file
            for(int j = 0; j < i; ++j) {
                fseek(readFiles[i], 0, SEEK_SET);
                fclose(readFiles[i]);
            }
            printf("One or many files are not the split files.\n");
            return;
        }
        readFiles[header->label - 1] = tmp;
    }

    for(int i = 0; i < totalFileNum; ++i) {
        uint8_t readContent[1000];
        while(!feof(readFiles[i])) {
            size_t readSize = fread(readContent, 1, 1000, readFiles[i]);
            fwrite(readContent, 1, readSize, target);
        }
        fseek(readFiles[i], 0, SEEK_SET);
        fclose(readFiles[i]);
    }
    fseek(target, 0, SEEK_SET);
}


int main(int argc, char *argv[]){

    if(argc < 3) {
        printf("Wrong parameter.\n");
        return 0;
    }
    
    if(strcmp(argv[1], "-s") == 0 ) {
        //Read file
        fstream inFile = fopen(argv[2], "r");
        if(inFile == NULL ) {
            printf("Cannot open file.\n");
            return 0;
        }
        size_t size = 1000;
        if(argc > 3) {
            if(strcmp(argv[3], "--size") == 0) {
                char *errPtr;
                size = strtol(argv[4], &errPtr, 10);
                if(*errPtr != '\0') {
                    fclose(inFile);
                    goto error_operation;
                }
            }else{
                fclose(inFile);
                goto error_operation;
            }
        }

        splitFile(inFile, argv[2], size);
        fclose(inFile);
    }else if (strcmp(argv[1], "-r") == 0) {
        fstream outFile = fopen(argv[2], "w");
        if(outFile == NULL ) {
            printf("Cannot open file.\n");
            return 0;
        }
        recoverFile(outFile, &argv[3], argc - 3);
        fclose(outFile);
        
    }else if (strcmp(argv[1], "--help") == 0) {
        printf("Split:\n./hw0401 -s [file] --size [Small File Size]\n The default small file size is 1000 bytes\n");
        printf("Recover:\n./hw0401 -r [output file] [small files]\n");    
    }else{
        error_operation:
        printf("Invalid operation.\n");
    }

    
}