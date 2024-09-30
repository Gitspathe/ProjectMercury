#ifndef COLORRGBA_H
#define COLORRGBA_H
#include <cstdint>

namespace Common
{
    struct ColorRGB;

    struct ColorRGBA
    {
        uint8_t r, g, b, a;

        explicit ColorRGBA(const uint8_t r = 0, const uint8_t g = 0, const uint8_t b = 0, const uint8_t a = 255)
            : r(r), g(g), b(b), a(a) {}

        static const ColorRGBA Clear;
        static const ColorRGBA Black;
        static const ColorRGBA White;
        static const ColorRGBA Red;
        static const ColorRGBA Green;
        static const ColorRGBA Blue;

        ColorRGBA operator+(const ColorRGBA& rhs) const;
        ColorRGBA operator-(const ColorRGBA& rhs) const;
        ColorRGBA operator*(const ColorRGBA& rhs) const;
        ColorRGBA operator/(const ColorRGBA& rhs) const;

        ColorRGBA operator+(const ColorRGB& rhs) const;
        ColorRGBA operator-(const ColorRGB& rhs) const;
        ColorRGBA operator*(const ColorRGB& rhs) const;
        ColorRGBA operator/(const ColorRGB& rhs) const;
    };
}

#endif //COLOR_H
