#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t b32;

typedef float f32;
typedef double f64;

#define U64Max UINT64_MAX
#define ArrayCount(array) (sizeof(array)/sizeof((array)[0]))
