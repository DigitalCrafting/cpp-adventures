#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "common/common_types.h"
#include "common/common_random.h"
#include "common/common_file.h"

void generate(int pairs) {
    f64 maxAllowedX = 180;
    f64 maxAllowedY = 90;

    f64 xCenter = 0;
    f64 yCenter = 0;
    f64 xRadius = maxAllowedX;
    f64 yRadius = maxAllowedY;

    RandomSeries seed = Seed(31);

    FILE* testDataFile = Open(pairs, "flex", "json");
    if (testDataFile) {
        fprintf(testDataFile, "{\"pairs\"\n: [\n");
        for (int i = 0; i < pairs; ++i) {
            f64 x0 = RandomDegree(&seed, xCenter, xRadius, maxAllowedX);
            f64 y0 = RandomDegree(&seed, yCenter, yRadius, maxAllowedY);
            f64 x1 = RandomDegree(&seed, xCenter, xRadius, maxAllowedX);
            f64 y1 = RandomDegree(&seed, yCenter, yRadius, maxAllowedY);

            const char* jsonSeparator = (i == (pairs - 1)) ? "\n" : ",\n";

            fprintf(testDataFile, "    {\"x0\": %.16f,\"y0\": %.16f,\"x1\": %.16f,\"y1\": %.16f}%s", x0, y0, x1, y1, jsonSeparator);
        }
    }

    fprintf(testDataFile, "]}\n");
}

int main(int argc, char **args) {
    const char* methodName = "uniform";
    u64 nmbrOfPairs = 100;
    u64 seedValue = 192837465;
    u64 clusterCountLeft = U64Max;
    
    if (argc == 1 || argc > 4) {
        fprintf(stderr, "Usage: %s [number of pairs] [random seed] [uniform/cluster]\n", args[0]);
        fprintf(stderr, "       %s [number of pairs] [random seed]\n", args[0]);
        fprintf(stderr, "       %s [number of pairs]\n", args[0]);
        return 1;
    }

    if (argc >= 2) {
        nmbrOfPairs = atoi(args[1]);
        if (nmbrOfPairs >= U64Max) {
            fprintf(stderr, "To avoid accidentally generating massive files, number of pairs must be less than %lu.\n", U64Max);
            return 1;
        }
    }

    if (argc >= 3) {
        seedValue = atoll(args[2]);
    }

    if (argc == 4) {
        methodName = args[3];

        if (strcmp(methodName, "cluster") == 0) {
            clusterCountLeft = 0;
        } else if (strcmp(methodName, "uniform") != 0) {
            methodName = "uniform";
            fprintf(stderr, "WARNING: Unrecognized method name. Using 'uniform'.\n");
        }
    }


    fprintf(stdout, "Method: %s\n", methodName);
    fprintf(stdout, "Random seed: %lu\n", seedValue);
    fprintf(stdout, "Pair count: %lu\n", nmbrOfPairs);

    generate(nmbrOfPairs);
    return 0;
}
