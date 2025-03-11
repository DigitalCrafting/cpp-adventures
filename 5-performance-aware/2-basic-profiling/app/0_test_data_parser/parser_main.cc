#include <iostream>

#include "haversine.h"
#include "profiler.h"
#include "config.hpp"
#include "common_random.h"
#include "common_file.h"

/*
 * Parses a file with longitudes and latitudes pairs, for testing haversine performance, in JSON format.
 *
 * File format:
 * {
 *  "pairs": [
 *      {"x0": <value_1>, "y0": <value_2>, "x1": <value_3>, "y1": <value_4>},
 *      .
 *      .
 *      .
 *  ]
 * }
 * */
void parse_test_data() {
    std::cout << project_name << '\n';
    std::cout << project_version << '\n';

    print_hello_haversine();
    print_hello_world();
}

int main() {
    parse_test_data();

    return 0;
}
