#ifndef COLOR3_H
#define COLOR3_H
#include <cstdint>

namespace Common
{
    struct ColorRGB
    {
        uint8_t r, g, b;

        explicit ColorRGB(const uint8_t r = 0, const uint8_t g = 0, const uint8_t b = 0)
            : r(r), g(g), b(b) {}

        static const ColorRGB Black;
        static const ColorRGB White;
        static const ColorRGB Red;
        static const ColorRGB Green;
        static const ColorRGB Blue;
    };
}

#endif //COLOR3_H
