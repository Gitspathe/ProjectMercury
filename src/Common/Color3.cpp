#include "Color3.h"

namespace Common
{
    Color3::Color3(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    const Color3 Color3::Black(0, 0, 0);
    const Color3 Color3::White(255, 255, 255);
    const Color3 Color3::Red(255, 0, 0);
    const Color3 Color3::Green(0, 255, 0);
    const Color3 Color3::Blue(0, 0, 255);
}
