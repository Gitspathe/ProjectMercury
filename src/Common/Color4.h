#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace Common
{
    struct Color4
    {
        uint8_t r, g, b, a;

        explicit Color4(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

        static Color4 getClear()
        {
            return Color4(0, 0, 0, 0);
        }

        static Color4 getBlack()
        {
            return Color4(0, 0, 0, 255);
        }

        static Color4 getWhite()
        {
            return Color4(255, 255, 255, 255);
        }

        static Color4 getRed()
        {
            return Color4(255, 0, 0, 255);
        }

        static Color4 getGreen()
        {
            return Color4(0, 255, 0, 255);
        }

        static Color4 getBlue()
        {
            return Color4(0, 0, 255, 255);
        }
    };
}

#endif //COLOR_H
