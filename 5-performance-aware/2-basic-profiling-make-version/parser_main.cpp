#include <iostream>
#include <sys/stat.h>

#include "common/common_types.h"
#include "common/common_file.h"
#include "common/common_haversine.h"
#include "common/common_json.h"
#include "base_json_lookup_parser.cpp"

static Buffer ReadEntireFile(char *filename) {
    Buffer result = {};

    FILE *file = fopen(filename, "rb");
    if (file) {
        struct stat statStruct;
        stat(filename, &statStruct);

        result = AllocateBuffer(statStruct.st_size);
        if (result.data) {
            if(fread(result.data, result.count, 1, file) != 1) {
                fprintf(stderr, "ERROR: Unable to read \"%s\".\n", filename);
                FreeBuffer(&result);
            }
        }

        fclose(file);
    } else {
        fprintf(stderr, "ERROR: Unable to read \"%s\".\n", filename);
    }

    return result;
}

static f64 SumHaversineDistances(u64 pairCount, HaversinePair* pairs) {
    f64 sum = 0;
    f64 sumCoef = 1 / (f64)pairCount;
    for (u64 pairIndex = 0; pairIndex < pairCount; ++pairIndex) {
        HaversinePair pair = pairs[pairIndex];
        f64 earthRadius = 6372.8;
        f64 dist = ReferenceHaversine(pair.x0, pair.y0, pair.x1, pair.y1, earthRadius);
        sum += sumCoef * dist;
    }
    return sum;
}

int main(int argc, char **args) {
    Buffer file = ReadEntireFile(args[1]);

    u32 minimumJSONPairEncoding = 6*4;
    u64 maxPairCount = file.count / minimumJSONPairEncoding;

    HaversinePair *pairs = (HaversinePair*)malloc(maxPairCount * sizeof(HaversinePair));

    u64 pairCount = BaseParser::ParseHaversinePairs(file, maxPairCount, pairs);

    f64 sum = SumHaversineDistances(pairCount, pairs);

    fprintf(stdout, "Input size: %lu\n", file.count);
    fprintf(stdout, "Pair count: %lu\n", pairCount);
    fprintf(stdout, "Haversine sum: %.16f\n", sum);
    return 0;
}
