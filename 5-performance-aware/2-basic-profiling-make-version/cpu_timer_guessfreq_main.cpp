#include <cstdio>
#include <cstdlib>

#include "common/types.h"
#include "common/platform_metrics.h"

int main(int argc, char **args) {
    u64 millisecondsToWait = 1000;
    if (argc == 2) {
        millisecondsToWait = atol(args[1]);
    }

    u64 osFreq = GetOSTimerFreq();
    printf("    OS Freq: %lu (reported)\n", osFreq);

    u64 cpuStart = ReadCPUTimer();
    u64 osStart = ReadOSTimer();
    u64 osEnd = 0;
    u64 osElapsed = 0;
    u64 osWaitTime = osFreq * millisecondsToWait / 1000;

    while (osElapsed < osWaitTime) {
        osEnd = ReadOSTimer();
        osElapsed = osEnd - osStart;
    }

    u64 cpuEnd = ReadCPUTimer();
    u64 cpuElapsed = cpuEnd - cpuStart;
    u64 cpuFreq = 0;

    if (osElapsed) {
        cpuFreq = osFreq * cpuElapsed / osElapsed;
    }

    printf("    OS Timer: %lu -> %lu = %lu elapsed\n", osStart, osEnd, osElapsed);
    printf("OS Seconds: %.4f\n", (f64)osElapsed/(f64)osFreq);
    printf("    CPU Timer: %lu -> %lu = %lu elapsed\n", cpuStart, cpuEnd, cpuElapsed);
    printf("    CPU Freq: %lu (guessed)\n", cpuFreq);

    return 0;
}
