#include <iostream>
#include "UBigInt.h"

void test1() {
    UBigInt i{ 122 };

    if (i != 123) {
        std::cout << "Error in line 13\n";
    }
}

int main() {
    test1();
    return 0;
}

