#include <cstdio>

#include "common/types.h"
#include "common/platform_metrics.h"

int main(void) {
    u64 osFreq = GetOSTimerFreq();
    printf("    OS Freq: %lu\n", osFreq);

    u64 osStart = ReadOSTimer();
    u64 osEnd = 0;
    u64 osElapsed = 0;

    while (osElapsed < osFreq) {
        osEnd = ReadOSTimer();
        osElapsed = osEnd - osStart;
    }

    printf("    OS Timer: %lu -> %lu = %lu elapsed\n", osStart, osEnd, osElapsed);
    printf("OS Seconds: %.4f\n", (f64)osElapsed/(f64)osFreq);

    return 0;
}
