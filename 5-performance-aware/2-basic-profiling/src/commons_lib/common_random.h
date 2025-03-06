#pragma once

#include "common_types.h"

struct RandomSeries {
    u64 A, B, C, D;
};

static u64 RotateLeft(u64 V, int shift) {
    u64 result = ((V << shift) | (V >> (64 - shift)));
    return result;
}

static u64 RandomU64(RandomSeries *series) {
    u64 A = series->A;
    u64 B = series->B;
    u64 C = series->C;
    u64 D = series->D;

    u64 E = A - RotateLeft(B, 27);

    A = (B ^ RotateLeft(C, 17));
    B = (C + D);
    C = (D + E);
    D = (E + A);

    series->A = A;
    series->B = B;
    series->C = C;
    series->D = D;

    return D;
}

static RandomSeries Seed(u64 value) {
    RandomSeries series = {};

    series.A = 0xf1ea5eed;
    series.B = value;
    series.C = value;
    series.D = value;

    u32 count = 20;
    while(count--) {
        RandomU64(&series);
    }

    return series;
}

static f64 RandomInRange(RandomSeries *series, f64 min, f64 max) {
    f64 t = (f64) RandomU64(series) / (f64)U64Max;
    f64 result = (1.0 - t)*min + t*max;

    return result;
}
