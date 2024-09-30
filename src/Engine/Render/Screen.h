#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include <cmath>
#include "../Common/ColorRGB.h"
#include "../Common/RectF.h"

namespace Engine::Render
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

        static std::unique_ptr<Screen> create(const int width, const int height)
        {
            return std::make_unique<Screen>(width, height);
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
