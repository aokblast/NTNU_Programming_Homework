#include "all.h"
#include "linux_header.h"


typedef struct _cpuUsage{
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t stealstolen;
    uint64_t guest;
} cpuUsage;


int main(){
    cpuUsage presentCpuInfo, prevCpuInfo;
    fstream cpuInfoFile;
    string cpuNumber;
    while(1) {
        cpuInfoFile = fopen("/proc/stat", "r");
        fscanf(cpuInfoFile, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld", cpuNumber,  &prevCpuInfo.user, &prevCpuInfo.nice, &prevCpuInfo.system, &prevCpuInfo.idle, &prevCpuInfo.iowait, &prevCpuInfo.irq, &prevCpuInfo.softirq, &prevCpuInfo.stealstolen, &prevCpuInfo.guest);
        uint64_t prevTotal = prevCpuInfo.user + prevCpuInfo.nice + prevCpuInfo.system + prevCpuInfo.idle + prevCpuInfo.iowait + prevCpuInfo.irq + prevCpuInfo.softirq + prevCpuInfo.stealstolen + prevCpuInfo.guest; 
        fclose(cpuInfoFile);
        sleep(1);
        cpuInfoFile = fopen("/proc/stat", "r");
        fscanf(cpuInfoFile, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld", cpuNumber,  &presentCpuInfo.user, &presentCpuInfo.nice, &presentCpuInfo.system, &presentCpuInfo.idle, &presentCpuInfo.iowait, &presentCpuInfo.irq, &presentCpuInfo.softirq, &presentCpuInfo.stealstolen, &presentCpuInfo.guest);
        uint64_t presentTotal = presentCpuInfo.user + presentCpuInfo.nice + presentCpuInfo.system + presentCpuInfo.idle + presentCpuInfo.iowait + presentCpuInfo.irq + presentCpuInfo.softirq + presentCpuInfo.stealstolen + presentCpuInfo.guest; 
        printf("Cpu: %lf\n", (100*((presentTotal - prevTotal)-(presentCpuInfo.idle - prevCpuInfo.idle))) / (double)(presentTotal - prevTotal));
        fclose(cpuInfoFile);
    }
}