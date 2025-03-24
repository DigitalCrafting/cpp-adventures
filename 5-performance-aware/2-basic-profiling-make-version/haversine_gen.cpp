#include <iostream>
#include <cstdlib>
#include <cstdint>

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
    int nmbrOfPairs = 100;

    if (argc == 2) {
        nmbrOfPairs = atoi(args[1]);
        if (nmbrOfPairs >= U64Max) {
            fprintf(stderr, "To avoid accidentally generating massive files, number of pairs must be less than %lu.\n", U64Max);
            return 1;
        }
    }


    generate(nmbrOfPairs);
    return 0;
}
