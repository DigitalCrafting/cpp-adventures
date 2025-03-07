#include <iostream>

#include "haversine.h"
#include "profiler.h"
#include "config.hpp"
#include "common_random.h"



/*
 * Generates a file with longitudes and latitudes pairs, for testing haversine performance, in JSON format.
 *
 * Final file format:
 * {
 *  "pairs": [
 *      {"x0": <value_1>, "y0": <value_2>, "x1": <value_3>, "y1": <value_4>},
 *      .
 *      .
 *      .
 *  ]
 * }
 * */
void generate_test_data(int pairs) {
    f64 maxAllowedX = 180;
    f64 maxAllowedY = 90;

    f64 xCenter = 0;
    f64 yCenter = 0;
    f64 xRadius = maxAllowedX;
    f64 yRadius = maxAllowedY;

    RandomSeries seed = Seed(31);

    for (int i = 0; i < pairs; i++) {
        printf("x: %f, ", RandomDegree(&seed, xCenter, xRadius, maxAllowedX));
        printf("y: %f\n", RandomDegree(&seed, yCenter, yRadius, maxAllowedY));
    }
}

int main() {
    std::cout << project_name << '\n';
    std::cout << project_version << '\n';

    print_hello_haversine();
    print_hello_world();

    generate_test_data(10);

    return 0;
}
