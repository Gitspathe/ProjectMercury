#ifndef COLOR3_H
#define COLOR3_H
#include <cstdint>

namespace Common
{
    struct Color3
    {
        uint8_t r, g, b;

        explicit Color3(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);

        static Color3 getBlack()
        {
            return Color3(0, 0, 0);
        }

        static Color3 getWhite()
        {
            return Color3(255, 255, 255);
        }

        static Color3 getRed()
        {
            return Color3(255, 0, 0);
        }

        static Color3 getGreen()
        {
            return Color3(0, 255, 0);
        }

        static Color3 getBlue()
        {
            return Color3(0, 0, 255);
        }
    };
}

#endif //COLOR3_H
