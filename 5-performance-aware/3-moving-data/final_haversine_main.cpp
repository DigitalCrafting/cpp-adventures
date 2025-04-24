#include <cstdio>
#include <sys/stat.h>

#include "common/types.h"
#include "common/file.h"
#include "common/haversine.h"
#include "final_timeblock_json_lookup_parser.cpp"

static Buffer ReadEntireFile(char *filename) {
    TimeFunction;
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
    TimeFunction;
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
    BeginProfile();
    if (argc == 1 || argc > 3) {
        fprintf(stderr, "Usage: %s [haversine_input.json]\n", args[0]);
        fprintf(stderr, "       %s [haversine_input.json] [answers.f64]\n", args[0]);
        return 1;
    }

    Buffer file = ReadEntireFile(args[1]);

    u32 minimumJSONPairEncoding = 6*4;
    u64 maxPairCount = file.count / minimumJSONPairEncoding;

    if (!maxPairCount) {
        fprintf(stderr, "ERROR: Mafromed input json");
        return 1;
    }

    Buffer parsedValues = AllocateBuffer(maxPairCount * sizeof(HaversinePair));
    HaversinePair *pairs = (HaversinePair*)parsedValues.data; 

    u64 pairCount = BaseParser::ParseHaversinePairs(file, maxPairCount, pairs);

    f64 sum = SumHaversineDistances(pairCount, pairs);

    fprintf(stdout, "Input size: %lu\n", file.count);
    fprintf(stdout, "Pair count: %lu\n", pairCount);
    fprintf(stdout, "Haversine sum: %.16f\n", sum);
    
    if (argc == 3) {
        Buffer answersFile = ReadEntireFile(args[2]);
        if (answersFile.count >= sizeof(f64)) {
            f64* answerValues = (f64*)answersFile.data;
            
            fprintf(stdout, "\nValidation:\n");

            u64 refAnswerCount = ((answersFile.count - sizeof(f64)) / sizeof(f64));
            if (pairCount != refAnswerCount) {
                fprintf(stdout, "FAILED - pair count doesn't match %lu.\n", refAnswerCount);
            }

            f64 refSum = answerValues[refAnswerCount];
            fprintf(stdout, "Reference sum: %.16f", refSum);
            fprintf(stdout, "Difference: %.16f\n", sum - refSum);
            fprintf(stdout, "\n");
        }

        FreeBuffer(&answersFile);
    }

    

    FreeBuffer(&parsedValues);
    FreeBuffer(&file);

    EndAndPrintProfile();
    return 0;
}
