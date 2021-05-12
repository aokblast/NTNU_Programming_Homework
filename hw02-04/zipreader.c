#include "zipreader.h"
#include "all.h"


bool is_header(uint32_t header, uint32_t signature);
void zip_free(zipobj *zipFile);

zipobj *zip_read(const char fileName[]) {
    //Read file and check for error
    if(strlen(fileName) <= 4 || strcmp(&fileName[strlen(fileName) - 4], ".zip") != 0) {
        return NULL;
    }
    zipobj *zipFile = calloc(1, sizeof(zipobj));
    zipFile->source_file = fopen(fileName, "rb");

    if(zipFile->source_file == NULL) {
        zip_close(zipFile);
        printf("File not found.\n");
        return NULL;
    }

    //read eocd and central file_dir header
    fseek(zipFile->source_file, 0 , SEEK_END);
    uint64_t offset = 0;
    do{
        fseek(zipFile->source_file, -(offset + EOCD_HEADER_SIZE), SEEK_END);
        fread(&zipFile->end_record, EOCD_HEADER_SIZE, 1, zipFile->source_file);
        ++offset;
        if(ftell(zipFile->source_file) == EOCD_HEADER_SIZE) {
            goto error_header;
        }
    } while(!is_header(zipFile->end_record.signature, EOCD_HEADER_SIGNATURE));
    zipFile->end_record.comment = calloc(zipFile->end_record.comment_length, sizeof(char));
    fread(zipFile->end_record.comment, sizeof(char), zipFile->end_record.comment_length, zipFile->source_file);
    

    //Read file info
    fseek(zipFile->source_file, zipFile->end_record.central_dir_offset, SEEK_SET);

    zipFile->central_header = calloc(zipFile->end_record.central_dir_num_total, sizeof(zip_dir_header));

    for(int i = 0; i < zipFile->end_record.central_dir_num_total; ++i) {
        fread(&zipFile->central_header[i], CENTRAL_DIR__HEADER_SIZE, 1, zipFile->source_file);
        if(!is_header(zipFile->central_header[i].signature, CENTRAL_DIR_HEADER_SIGNATURE)) {
            printf("Wrong central header.\n");
            zip_close(zipFile);
            return NULL;
        }
        
        zipFile->central_header[i].fileName = calloc(zipFile->central_header[i].name_length, sizeof(char));
        zipFile->central_header[i].field = calloc(zipFile->central_header[i].field_length, sizeof(uint8_t));
        zipFile->central_header[i].comment = calloc(zipFile->central_header[i].comment_length, sizeof(char));

        fread(zipFile->central_header[i].fileName, zipFile->central_header[i].name_length, sizeof(char), zipFile->source_file);
        fread(zipFile->central_header[i].field, zipFile->central_header[i].field_length, sizeof(uint8_t), zipFile->source_file);
        fread(zipFile->central_header[i].comment, zipFile->central_header[i].comment_length, sizeof(char), zipFile->source_file);

    }

    fseek(zipFile->source_file, 0, SEEK_SET);

    return zipFile;

    error_header:
        printf("Cannot find eocd\n");
        zip_close(zipFile);
        return NULL;
}


bool is_header(uint32_t header, uint32_t signature) {
    return header == signature;
}

void zip_close(zipobj* zipFile) {
    if(zipFile == NULL) return;

    //free eocd
    if(zipFile->end_record.comment != NULL) {
        free(zipFile->end_record.comment);
    }
    //free central_dir
    for(int i = 0; i < zipFile->end_record.central_dir_num_total; ++i) {
        if(zipFile->central_header[i].comment != NULL) {
            free(zipFile->central_header[i].comment);
        }
        if(zipFile->central_header[i].fileName != NULL) {
            free(zipFile->central_header[i].fileName);
        }
        if(zipFile->central_header[i].field != NULL) {
            free(zipFile->central_header[i].field);
        }
    }

    //free files
    if(zipFile->fileNumber != 0) {
        for(int i = 0; i < zipFile->fileNumber; ++i) {
            zip_local_header *file = &zipFile->files[i];
            if(file->field != NULL) {
                free(file->field);
            }
            if(file->fileName != NULL) {
                free(file->fileName);
            }
        }

    }

    //close file
    if(zipFile->source_file != NULL) {
        fclose(zipFile->source_file);
    }

    if(zipFile != NULL) {
        free(zipFile);
    }
    
}

void show_file_info(zip_dir_header *file) {
    zip_dir_header *dir_header = file;
    printf("FileName:          %s\n", dir_header->fileName);
    printf("FileComment:       %s\n", dir_header->comment);
    printf("Signature:         %d\n", dir_header->signature);
    printf("Version:           %d\n", dir_header->version);
    printf("Min-Version:       %d\n", dir_header->min_version);
    printf("Flag:              %d\n", dir_header->flag);
    printf("Compression:       %d\n", dir_header->compression);
    printf("Modification-Time: %d\n", dir_header->modify_time);
    printf("Modification-Date: %d\n", dir_header->modify_date);
    printf("CRC32:             %d\n", dir_header->crc32);
    printf("Compressed-size:   %d\n", dir_header->compress_size);
    printf("Uncompressed-size: %d\n", dir_header->uncompressed_size);
}

int getLayer(char *fileDir) {
    int layer = 1;
    fileDir = strchr(fileDir, '/');
    if(fileDir == NULL) return layer;
    ++fileDir;
    while(fileDir != NULL) {
        fileDir = strchr(fileDir, '/');
        if(fileDir == NULL) break;
        ++layer, ++fileDir;
    }

    return layer;
}

void show_zip_file_list(zipobj *zipFile){
    for(int i = 0; i < zipFile->end_record.central_dir_num_total; ++i) {
        int layer = getLayer(zipFile->central_header[i].fileName);
        if(zipFile->central_header[i].fileName[strlen(zipFile->central_header[i].fileName) - 1] == '/'){
            for(int i = 0; i < layer - 1; ++i) printf("\t");
            char *iter = strrchr(zipFile->central_header[i].fileName, '/');
            if(iter == NULL) iter = zipFile->central_header[i].fileName;
            else {
                
                *iter = '\0';
                iter = strrchr(zipFile->central_header[i].fileName, '/');
                if( iter == NULL) {
                    iter = zipFile->central_header[i].fileName;
                }else {
                    ++iter;
                }
            }
            
            printf("+-- %s\n", iter);
        }else {
            for(int i = 0; i < layer; ++i) printf("\t");
            char *iter = strrchr(zipFile->central_header[i].fileName, '/');
            if(iter == NULL) iter = zipFile->central_header[i].fileName;
            else ++iter;
            printf("+-- %s\n", iter);
        }
    }
}