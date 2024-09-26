#include "Screen.h"
#include "../Common/Color.h"

namespace Render
{
    Screen::Screen(const int width, const int height)
    {
        this->width = width;
        this->height = height;
        buffer = std::vector<std::vector<Common::Color>>(height);
        for(int i = 0; i < height; i++){
            buffer[i] = std::vector<Common::Color>(width);
        }
        clear(Common::Color::getBlack());
    }

    void Screen::clear(const Common::Color color)
    {
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                buffer[i][j] = color;
            }
        }
    }

    void Screen::set(const int x, const int y, const Common::Color color)
    {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return;

        buffer[y][x] = color;
    }

    Common::Color Screen::get(const int x, const int y) const {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return Common::Color::getWhite();

        return buffer[y][x];
    }

    int Screen::getHeight() const
    {
        return height;
    }

    int Screen::getWidth() const
    {
        return width;
    }

}
