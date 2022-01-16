#include "all.h"
#include "zipreader.h"

int main(int argc, char *argv[]){
    zipobj *zipFile = zip_read(argv[1]);
    if(zipFile == NULL) {
        return 0;
    }
    show_zip_file_list(zipFile);
    zip_close(zipFile);
}