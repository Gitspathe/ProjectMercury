#ifndef COLORRGB_H
#define COLORRGB_H
#include <cstdint>

namespace Engine::Common
{
    struct ColorRGBA;

    struct ColorRGB //: BaseColor
    {
        uint8_t r, g, b;

        explicit ColorRGB(const uint8_t r = 0, const uint8_t g = 0, const uint8_t b = 0)
            : r(r), g(g), b(b) {}

        static const ColorRGB Black;
        static const ColorRGB White;
        static const ColorRGB Red;
        static const ColorRGB Green;
        static const ColorRGB Blue;

        ColorRGB operator+(const ColorRGB& rhs) const;
        ColorRGB operator-(const ColorRGB& rhs) const;
        ColorRGB operator*(const ColorRGB& rhs) const;
        ColorRGB operator/(const ColorRGB& rhs) const;

        ColorRGB operator+(const ColorRGBA& rhs) const;
        ColorRGB operator-(const ColorRGBA& rhs) const;
        ColorRGB operator*(const ColorRGBA& rhs) const;
        ColorRGB operator/(const ColorRGBA& rhs) const;
    };
}
#endif //COLOR3_H
