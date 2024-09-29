#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include <cmath>
#include "../Common/ColorRGB.h"
#include "../Common/RectF.h"

namespace Render
{
    class Screen
    {
    private:
        int width, height;

    public:
        Screen(const int width, const int height)
        {
            this->width = width;
            this->height = height;
        }

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }
    };
}

#endif //SCREEN_H
