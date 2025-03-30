#include <cstdio>

#include "common/types.h"
#include "common/platform_metrics.h"

int main(void) {
    u64 osFreq = GetOSTimerFreq();
    printf("    OS Freq: %lu\n", osFreq);

    u64 cpuStart = ReadCPUTimer();
    u64 osStart = ReadOSTimer();
    u64 osEnd = 0;
    u64 osElapsed = 0;

    while (osElapsed < osFreq) {
        osEnd = ReadOSTimer();
        osElapsed = osEnd - osStart;
    }

    u64 cpuEnd = ReadCPUTimer();
    u64 cpuElapsed = cpuEnd - cpuStart;

    printf("    OS Timer: %lu -> %lu = %lu elapsed\n", osStart, osEnd, osElapsed);
    printf("OS Seconds: %.4f\n", (f64)osElapsed/(f64)osFreq);
    printf("    CPU Timer: %lu -> %lu = %lu elapsed\n", cpuStart, cpuEnd, cpuElapsed);

    return 0;
}
