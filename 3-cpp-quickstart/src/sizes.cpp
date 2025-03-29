#include <iostream>

int main(int argc, char** args) {
    std::cout << "Sizes of types:\n";
    std::cout << "int               " << sizeof(int) << "\n";
    std::cout << "char              " << sizeof(char) << "\n";
    std::cout << "unsigned int      " << sizeof(unsigned int) << "\n";
    std::cout << "long              " << sizeof(long) << "\n";
    std::cout << "long long         " << sizeof(long long) << "\n";
    std::cout << "double            " << sizeof(double) << "\n";
    std::cout << "float             " << sizeof(float) << "\n";
    std::cout << "bool              " << sizeof(bool) << "\n";
    return 0;
}
