#include <iostream>
#include <sys/stat.h>

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

int main(int argc, char **args) {
    Buffer file = ReadEntireFile(args[1]);

    std::cout << "Read file size: " << file.count << '\n';

    HaversinePair *pairs = (HaversinePair*)malloc(10 * sizeof(HaversinePair));

    BaseParser::ParseHaversinePairs(file, 10, pairs);

    std::cout << "First parsed pair: " << pairs[0].x0 << " - " << pairs[0].y0 << " - " << pairs[0].x1 << " - " << pairs[0].y1 << '\n';
    return 0;
}
