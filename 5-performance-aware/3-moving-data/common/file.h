#pragma once

#include "types.h"
#include <cstddef>
#include <cstdlib>
#include <cstdio>

/* -------------- Buffer -------------- */
#define CONSTANT_STRING(String) {sizeof(String) - 1, (u8 *)(String)}

struct Buffer {
    size_t count;
    u8 *data;
};

static b32 IsInBounds(Buffer source, u64 at) {
    b32 result = (at < source.count);
    return result;
}

static b32 AreEqual(Buffer a, Buffer b) {
    if (a.count != b.count) {
        return false;
    }

    for (u64 index = 0; index < a.count; ++index) {
        if (a.data[index] != b.data[index]) {
            return false;
        }
    }

    return true;
}

static Buffer AllocateBuffer(size_t count) {
    Buffer result = {};
    result.data = (u8 *) malloc(count);
    if (result.data) {
        result.count = count;
    } else {
        fprintf(stderr, "ERROR: Unable to allocate %lu bytes.}\n", count);
    }

    return result;
}

static void FreeBuffer(Buffer *buffer) {
    if (buffer->data) {
        free(buffer->data);
    }
    *buffer = {};
}


/* -------------- File Methods -------------- */
static FILE* Open(long long unsigned pairCount, const char* label, const char *extension) {
    char temp[256];
    sprintf(temp, "data_%llu_%s.%s", pairCount, label, extension);
    FILE* result = fopen(temp, "wb");
    if (!result) {
        fprintf(stderr, "Unable to open \"%s\" for writing.\n", temp);
    }
    return result;
}

