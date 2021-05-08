#ifndef ZIPREADER
#define ZIPREADER
#include "all.h"

#define LOCAL_HEADER_SIGNATURE 0x04034b50
#define CENTRAL_DIR_HEADER_SIGNATURE 0x02014b50
#define EOCD_HEADER_SIGNATURE 0x06054b50
#define EOCD_HEADER_SIZE sizeof(zip_eocd) - sizeof(char *)
#define CENTRAL_DIR__HEADER_SIZE sizeof(zip_dir_header) - sizeof(char *) - sizeof(char *) - sizeof (uint8_t *) 
#define LOCAL_HEADER_SIZE sizeof(zip_basic_header)

typedef struct _zip_basic_header {
    uint32_t signature;
    uint16_t min_version;
    uint16_t flag;
    uint16_t compression;
    uint16_t modify_time;
    uint16_t midify_date;
    uint32_t crc32;
    uint32_t compress_size;
    uint32_t uncompressed_size;
    uint16_t name_length;
    uint16_t field_length;
} __attribute__ ((__packed__)) zip_basic_header;

typedef struct _zip_local_header{
    zip_basic_header origHeader;
    char *fileName;
    uint8_t *field;
} __attribute__ ((__packed__)) zip_local_header;


typedef struct _zip_dir_header
{
    uint32_t signature;
    uint16_t version;
    uint16_t min_version;
    uint16_t flag;
    uint16_t compression;
    uint16_t modify_time;
    uint16_t modify_date;
    uint32_t crc32;
    uint32_t compress_size;
    uint32_t uncompressed_size;
    uint16_t name_length;
    uint16_t field_length;
    uint16_t comment_length;
    uint16_t disk_num;
    uint16_t inter_attr;
    uint32_t exter_attr;
    uint32_t file_offset; 
    char *fileName;
    uint8_t *field;
    char *comment;    
} __attribute__ ((__packed__)) zip_dir_header;

typedef struct _zip_eocd {
    uint32_t signature;
    uint16_t disk_num;
    uint16_t disk_start;
    uint16_t central_dir_num_present;
    uint16_t central_dir_num_total;
    uint32_t central_dir_size;
    uint32_t central_dir_offset;
    uint16_t comment_length;
    char *comment;
} __attribute__ ((__packed__)) zip_eocd;

typedef struct _zipobj {
    struct _zip_eocd end_record;
    struct _zip_dir_header *central_header;
    struct _zip_local_header *files;
    fstream source_file;
    uint32_t fileNumber;
} __attribute__ ((__packed__)) zipobj;

zipobj *zip_read(const char fileName[]);
void zip_close(zipobj *zipFile);
void show_zip_file_list(zipobj *zipFile);


#endif