#include "Color4.h"

namespace Common
{
    Color4::Color4(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    const Color4 Color4::Clear(0, 0, 0, 0);
    const Color4 Color4::Black(0, 0, 0, 255);
    const Color4 Color4::White(255, 255, 255, 255);
    const Color4 Color4::Red(255, 0, 0, 255);
    const Color4 Color4::Green(0, 255, 0, 255);
    const Color4 Color4::Blue(0, 0, 255, 255);
}
