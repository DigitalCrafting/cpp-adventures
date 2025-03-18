#include <iostream>
#include <sys/stat.h>

#include "haversine.h"
#include "profiler.h"
#include "config.hpp"
#include "common_random.h"
#include "common_json.h"
#include "base_json_lookup_parser.h"

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

    HaversinePair pairs = {};

    BaseParser::ParseHaversinePairs({}, 0, &pairs);
}

int main() {
    parse_test_data();

    Buffer file = ReadEntireFile("/home/kamil/Projects/github/cpp-adventures/5-performance-aware/2-basic-profiling/cmake-build-debug/app/0_test_data_generator/data_10_flex.json");

    std::cout << "Read file size: " << file.count << '\n';

    return 0;
}
