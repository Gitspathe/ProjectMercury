#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace Common
{
    struct Color
    {
        uint8_t r, g, b, a;

        explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
    };
}

#endif //COLOR_H
