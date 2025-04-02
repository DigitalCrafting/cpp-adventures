#pragma once
#include "../common/platform_metrics.h"
#include <cstdio>

struct ProfileAnchor {
    u64 tscElapsed;
    u64 hitCount;
    const char *label;   
};

struct Profiler {
    ProfileAnchor anchors[4096];

    u64 startTsc;
    u64 endTsc;
};

static Profiler GLOBAL_PROFILER;

struct ProfileBlock {
    const char* label;
    u64 startTsc;
    u32 anchorIndex;

    ProfileBlock(const char* label_, u32 anchorIndex_) {
        label = label_;
        anchorIndex = anchorIndex_;
        startTsc = ReadCPUTimer();
    }

    ~ProfileBlock() {
        u64 elapsed = ReadCPUTimer() - startTsc;

        ProfileAnchor* anchor = GLOBAL_PROFILER.anchors + anchorIndex;
        anchor->tscElapsed += elapsed;
        ++anchor->hitCount;
        anchor->label = label;
    }
};

#define NameConcat2(A, B) A##B
#define NameConcat(A, B) NameConcat2(A, B)
#define TimeBlock(Name) ProfileBlock NameConcat(Block, __LINE__)(Name, __COUNTER__ + 1);
#define TimeFunction TimeBlock(__func__)

static void PrintTimeElapsed(u64 totalTSCElapsed, ProfileAnchor* anchor) {
    u64 elapsed = anchor->tscElapsed;
    f64 percent = 100.0 * ((f64)elapsed / (f64)totalTSCElapsed);
    printf("  %s[%lu]: %lu (%.2f%%)\n", anchor->label, anchor->hitCount, elapsed, percent);
}

static void BeginProfile(void) {
    GLOBAL_PROFILER.startTsc = ReadCPUTimer();
}

static void EndAndPrintProfile() {
    GLOBAL_PROFILER.endTsc = ReadCPUTimer();
    u64 cpuFreq = EstimateCPUTimerFreq();

    u64 totalCPUElapsed = GLOBAL_PROFILER.endTsc - GLOBAL_PROFILER.startTsc;

    if (cpuFreq) {
        printf("\nTotal time: %0.4fms (CPU freq %lu)\n", 1000.0 * (f64)totalCPUElapsed / (f64)cpuFreq, cpuFreq);
    }

    for (u32 anchorIndex = 0; anchorIndex < ArrayCount(GLOBAL_PROFILER.anchors); ++anchorIndex) {
        ProfileAnchor *anchor = GLOBAL_PROFILER.anchors + anchorIndex;
        if (anchor->tscElapsed) {
            PrintTimeElapsed(totalCPUElapsed, anchor);
        }
    }
}

