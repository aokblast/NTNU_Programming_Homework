#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#define FMT_BARE "%06llx %02x %02x\n"
#define FMT_PRETTY "%06llx: %02x -> %02x\n"

int main(int argc, char *argv[]) {
    char *fmt = FMT_BARE;

    int opt;
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p': fmt = FMT_PRETTY; break;
            default:
                fprintf(stderr, "Usage: %s [-p]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    char buf[255];
    off_t offset;
    unsigned int left, right;

    for (unsigned int ln = 1; fgets(buf, 255, stdin) != NULL; ln++) {
        if (sscanf(buf, "%llu %o %o", &offset, &left, &right) == 3) {
            printf(fmt, offset, left, right);
        } else {
            printf("Unexpected input at line %u.\n", ln);
            return EXIT_FAILURE;
        }
    }

    return 0;
}