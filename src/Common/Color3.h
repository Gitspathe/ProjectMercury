#ifndef COLOR3_H
#define COLOR3_H
#include <cstdint>

namespace Common
{
    struct Color3
    {
        uint8_t r, g, b;

        explicit Color3(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);

        static const Color3 Black;
        static const Color3 White;
        static const Color3 Red;
        static const Color3 Green;
        static const Color3 Blue;
    };
}

#endif //COLOR3_H
