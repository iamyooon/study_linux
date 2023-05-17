#include <iostream>
extern "C" {
#include "addition.h"
}

int main() {
    int result = addition(3, 4);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
