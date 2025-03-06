#include <iostream>

#include "haversine.h"
#include "profiler.h"
#include "config.hpp"
#include "common_random.h"

int main() {
    std::cout << project_name << '\n';
    std::cout << project_version << '\n';

    print_hello_haversine();
    print_hello_world();

    RandomSeries seed = Seed(31);

    std::cout << RandomU64(&seed) << '\n';

    return 0;
}
