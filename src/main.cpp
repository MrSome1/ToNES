
#include <iostream>

#include "MicroProcessor.h"

int main(int argc, char* argv[]) {
    uint8_t a = 0x0f;
    uint8_t b = 0xff;
    uint16_t c = a;

    a -= b;
    c -= b;

    std::cout << "a - b: " << std::hex << a << std::endl;
    std::cout << "c - b: " << std::hex << c << std::endl;
    std::cout << "equal: " << (a == (uint8_t)c) << std::endl;

    return 0;
}