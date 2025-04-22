#pragma once

#include <cstdio>
#include "./types.h"

#if _WIN32

#include <intrin.h>
#include <windows.h>

static u64 GetOSTimerFreq(void) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

static u64 ReadOSTimer(void) {
    LARGE_INTEGER value;
    QueryPerformanceCounter(&value);
    return value.QuadPart;
}

#else

#include <x86intrin.h>
#include <sys/time.h>

static u64 GetOSTimerFreq(void) {
    return 1000000;
}

static u64 ReadOSTimer(void) {
    timeval value;
    gettimeofday(&value, 0);
    u64 result = GetOSTimerFreq() * (u64)value.tv_sec + (u64)value.tv_usec;
    return result;
}

#endif

inline u64 ReadCPUTimer(void) {
    return __rdtsc();
}

static u64 EstimateCPUTimerFreq(void) {

    u64 millisecondsToWait = 100;
    u64 osFreq = GetOSTimerFreq();

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

    return cpuFreq;
}

static void PrintTimeElapes(char const* label, u64 totalTSCElapsed, u64 begin, u64 end) {
    u64 elapsed = end - begin;
    f64 percent = 100.0 * ((f64)elapsed / (f64)totalTSCElapsed);
    printf("    %s: %lu (%.2f%%)\n", label, elapsed, percent);
}
