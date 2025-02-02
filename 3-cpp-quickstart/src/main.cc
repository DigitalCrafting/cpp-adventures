#include<iostream>

//#include "../include/my_lib.h"

int main() {
//    print_hello_world();
    int intVar = 5;

    int* intPointer = &intVar;

    int** pointerPointer = &intPointer;

    std::cout << "pointerPointer address -> pointerPointer value -> pointerPointer deref -> int" << "\n";
    std::cout << &pointerPointer << " -> " << pointerPointer << " -> " << *pointerPointer << " -> "  << *(*pointerPointer) << "\n";

    return 0;
}
