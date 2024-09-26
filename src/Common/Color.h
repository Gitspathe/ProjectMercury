#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace Common
{
    struct Color
    {
        uint8_t r, g, b, a;

        explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

        static Color getClear()
        {
            return Color(0, 0, 0, 0);
        }

        static Color getBlack()
        {
            return Color(0, 0, 0, 255);
        }

        static Color getWhite()
        {
            return Color(255, 255, 255, 255);
        }

        static Color getRed()
        {
            return Color(255, 0, 0, 255);
        }

        static Color getGreen()
        {
            return Color(0, 255, 0, 255);
        }

        static Color getBlue()
        {
            return Color(0, 0, 255, 255);
        }
    };
}

#endif //COLOR_H
