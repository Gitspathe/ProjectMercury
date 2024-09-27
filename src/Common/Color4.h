#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace Common
{
    struct Color4
    {
        uint8_t r, g, b, a;

        explicit Color4(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

        static const Color4 Clear;
        static const Color4 Black;
        static const Color4 White;
        static const Color4 Red;
        static const Color4 Green;
        static const Color4 Blue;
    };
}

#endif //COLOR_H
