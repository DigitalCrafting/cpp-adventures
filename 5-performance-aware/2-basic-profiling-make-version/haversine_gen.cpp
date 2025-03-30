#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "common/types.h"
#include "common/random.h"
#include "common/file.h"
#include "common/haversine.h"

void generate(int pairs, u64 seedValue, u64 clusterCountLeft, const char *methodName) {
    f64 maxAllowedX = 180;
    f64 maxAllowedY = 90;

    f64 xCenter = 0;
    f64 yCenter = 0;
    f64 xRadius = maxAllowedX;
    f64 yRadius = maxAllowedY;

    RandomSeries seed = Seed(seedValue);

    FILE* testDataFile = Open(pairs, "flex", "json");
    FILE* answersFile = Open(pairs, "haveranswer", "f64");
    if (testDataFile && answersFile) {
        fprintf(testDataFile, "{\"pairs\"\n: [\n");
        u64 clusterCountMax = 1 + (pairs / 64);
        f64 sum = 0;
        f64 sumCoef = 1.0 / (f64)pairs;

        for (int i = 0; i < pairs; ++i) {
            if (clusterCountLeft-- == 0) {
                clusterCountLeft = clusterCountMax;
                xCenter = RandomInRange(&seed, -maxAllowedX, maxAllowedX);
                yCenter = RandomInRange(&seed, -maxAllowedY, maxAllowedY);
                xRadius = RandomInRange(&seed, 0, maxAllowedX);
                yRadius = RandomInRange(&seed, 0, maxAllowedY);
            }

            f64 x0 = RandomDegree(&seed, xCenter, xRadius, maxAllowedX);
            f64 y0 = RandomDegree(&seed, yCenter, yRadius, maxAllowedY);
            f64 x1 = RandomDegree(&seed, xCenter, xRadius, maxAllowedX);
            f64 y1 = RandomDegree(&seed, yCenter, yRadius, maxAllowedY);

            f64 earthRadius = 6372.8;
            f64 haversineDistance = ReferenceHaversine(x0, y0, x1, y1, earthRadius);

            sum += sumCoef * haversineDistance;

            const char* jsonSeparator = (i == (pairs - 1)) ? "\n" : ",\n";

            fprintf(testDataFile, "    {\"x0\": %.16f,\"y0\": %.16f,\"x1\": %.16f,\"y1\": %.16f}%s", x0, y0, x1, y1, jsonSeparator);
            fwrite(&haversineDistance, sizeof(haversineDistance), 1, answersFile);
        }

        fprintf(testDataFile, "]}\n");
        fwrite(&sum, sizeof(sum), 1, answersFile);

        fprintf(stdout, "Method: %s\n", methodName);
        fprintf(stdout, "Random seed: %lu\n", seedValue);
        fprintf(stdout, "Pair count: %u\n", pairs);
        fprintf(stdout, "Expected sum: %.16f\n", sum);
    }

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


    generate(nmbrOfPairs, seedValue, clusterCountLeft, methodName);
    return 0;
}
