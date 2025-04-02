#include <iostream>

void hello() {
    std::cout << "Hello\n";
}

void runner(void(*func)()) {
    func();
}

int main() {

    runner(&hello);

    return 0;
}
