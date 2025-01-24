#include <iostream>

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

#include "my_lib.h"

void print_hello_world() {
    std::cout << "Hello world!\n";

    std::cout << "FMT:" << FMT_VERSION << "\n";

    std::cout << "CXXOPTS:" << CXXOPTS__VERSION_MAJOR << "." << CXXOPTS__VERSION_MINOR << "." << CXXOPTS__VERSION_PATCH
              << "\n";

    std::cout << "SPDLOG:" << SPDLOG_VER_MAJOR << "." << SPDLOG_VER_MINOR << "." << SPDLOG_VER_PATCH << "\n";
}

uint32_t factorial(uint32_t number)
{
    return number <= 1 ? 1 : factorial(number - 1) * number;
}

