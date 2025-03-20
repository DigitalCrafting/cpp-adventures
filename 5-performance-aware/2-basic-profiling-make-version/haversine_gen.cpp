#include <iostream>
#include <cstdlib>
#include <cstdint>

void generate(int nmbrOfPairs) {
    std::cout << "Hello" << nmbrOfPairs << '\n';
}

int main(int argc, char **args) {
    int nmbrOfPairs = 100;

    if (argc == 2) {
        nmbrOfPairs = atoi(args[1]);
        if (nmbrOfPairs >= UINT64_MAX) {
            fprintf(stderr, "To avoid accidentally generating massive files, number of pairs must be less than %lu.\n", UINT64_MAX);
            return 1;
        }
    }


    generate(nmbrOfPairs);
    return 0;
}
