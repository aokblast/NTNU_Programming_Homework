#include "all.h"
#include "san5pk_editor.h"
#include "bitMani.h"
#define s_Type heroinfo


int main () {
    s_Type readStruct[1];
    fstream readFile = fopen("/home/blast/sanguo/SAVEDATA.S5P", "rb");
    fseek(readFile, 7656, SEEK_SET);
    fread(readStruct, sizeof(s_Type), 1, readFile);
    printf("%d %d %d %d %d %d %d %d %d %u %u\n", readStruct->reputation, readStruct->exp, readStruct->soldier, readStruct->force, readStruct->wise, readStruct->policy, readStruct->charm, readStruct->train, readStruct->moral, readStruct->loyalty, readStruct->skills);
    fclose(readFile);
}