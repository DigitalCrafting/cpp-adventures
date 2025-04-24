#pragma once

#include <cstdio>

#include "../common/platform_metrics.h"

#ifndef PROFILER
#define PROFILER 0
#endif


#if PROFILER

struct ProfileAnchor {
    u64 tscElapsedInclusive;
    u64 tscElapsedExclusive;
    u64 hitCount;
    const char* name;
};

static ProfileAnchor GLOBAL_PROFILE_ANCHORS[4096];
static u32 GLOBAL_PARENT_ANCHOR;

struct ProfileBlock {
    const char* label;
    u64 startTsc;
    u64 oldTscElapsedInclusive;
    u32 anchorIndex;
    u32 parentIndex;

    ProfileBlock(const char* label_, u32 anchorIndex_) {
        anchorIndex = anchorIndex_;
        label = label_;
        parentIndex = GLOBAL_PARENT_ANCHOR;
        GLOBAL_PARENT_ANCHOR = anchorIndex;

        ProfileAnchor* anchor = GLOBAL_PROFILE_ANCHORS + anchorIndex;

        oldTscElapsedInclusive = anchor->tscElapsedInclusive;
        startTsc = ReadCPUTimer();
    }

    ~ProfileBlock() {
        ProfileAnchor* anchor = GLOBAL_PROFILE_ANCHORS + anchorIndex;
        ProfileAnchor* parent = GLOBAL_PROFILE_ANCHORS + parentIndex;
        GLOBAL_PARENT_ANCHOR = parentIndex;

        u64 elapsed = ReadCPUTimer() - startTsc;

        parent->tscElapsedExclusive -= elapsed;
        anchor->tscElapsedExclusive += elapsed;
        anchor->tscElapsedInclusive = oldTscElapsedInclusive + elapsed;
        anchor->name = label;
        ++anchor->hitCount;
    }
};

static void PrintTimeElapsed(u64 totalElapsedTime, ProfileAnchor* anchor) {
    f64 percent = 100.0 * ((f64)anchor->tscElapsedExclusive / (f64)totalElapsedTime);
    printf("    %s[%lu]: %lu (%.2f%%", anchor->name, anchor->hitCount, anchor->tscElapsedExclusive, percent);
    if (anchor->tscElapsedInclusive != anchor->tscElapsedExclusive) {
        f64 percentWithChildren = 100.0 * ((f64)anchor->tscElapsedInclusive / (f64)totalElapsedTime);
        printf(", %.2f%% w/children", percentWithChildren);
    }
    printf(")\n");
}

static void PrintAnchorData(u64 totalElapsedTime) {
    for (u32 anchorIndex = 0; anchorIndex < ArrayCount(GLOBAL_PROFILE_ANCHORS); ++anchorIndex) {
        ProfileAnchor* anchor = GLOBAL_PROFILE_ANCHORS + anchorIndex;
        if (anchor->tscElapsedInclusive) {
            PrintTimeElapsed(totalElapsedTime, anchor);
        } 
    }
}

#define NameConcat2(A, B) A##B
#define NameConcat(A, B) NameConcat2(A, B)
#define TimeBlock(Name) ProfileBlock NameConcat(Block, __LINE__)(Name, __COUNTER__ + 1);
#define ProfilerEndOfCompilationUnit static_assert(__COUNTER__ < ArrayCount(GLOBAL_PROFILE_ANCHORS), "Number of available profile points exceeded")

#else

#define TimeBlock(...)
#define PrintAnchorData(...)
#define ProfilerEndOfCompilationUnit

#endif

struct Profiler {
    u64 startTsc;
    u64 endTsc;
};
static Profiler GLOBAL_PROFILER;

#define TimeFunction TimeBlock(__func__)

static void BeginProfile(void) {
    GLOBAL_PROFILER.startTsc = ReadCPUTimer();
}

static void EndAndPrintProfile(void) {
    GLOBAL_PROFILER.endTsc = ReadCPUTimer();
    u64 cpuFreq = EstimateCPUTimerFreq();

    u64 totalElapsedTime = GLOBAL_PROFILER.endTsc - GLOBAL_PROFILER.startTsc;

    if (cpuFreq) {
        printf("\nTotal time: %0.4fms (CPU freq %lu)\n", 1000.0 * (f64)totalElapsedTime / (f64)cpuFreq, cpuFreq);        
    }

    PrintAnchorData(totalElapsedTime);
}



