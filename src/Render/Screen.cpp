#include "Screen.h"
#include <cmath>
#include "../Common/Color.h"

namespace Render
{
    Screen::Screen(const int width, const int height)
    {
        this->width = width;
        this->height = height;
        buffer = std::vector<Common::Color>(height * width);
        clear(Common::Color::getBlack());
    }

    void Screen::clear(const Common::Color color)
    {
        for(int i = 0; i < height * width; i++) {
            buffer[i] = color;
        }
    }

    void Screen::drawPoint(int size, int x, int y, Common::Color color, bool centered)
    {
        if(size == 1) {
            set(x, y, color);
            return;
        }
        int startX, startY, endX, endY;
        if(centered) {
            startX = x - size / 2;
            startY = y - size / 2;
            endX = x + size / 2;
            endY = y + size / 2;
        } else {
            startX = x - size;
            startY = y - size;
            endX = x ;
            endY = y ;
        }
        for(int x = startX; x < endX; ++x) {
            for(int y = startY; y < endY; ++y) {
                set(x, y, color);
            }
        }
    }

    void Screen::drawLine(int x0, int y0, int x1, int y1, Common::Color color) {
        // Calculate deltas
        int deltaX = abs(x1 - x0);
        int deltaY = abs(y1 - y0);

        // Determine the direction of the line
        int signX = (x0 < x1) ? 1 : -1;
        int signY = (y0 < y1) ? 1 : -1;

        // Bresenham's algorithm variables
        int error = deltaX - deltaY;

        while(true) {
            // Set the pixel at the current pointa
            set(x0, y0, color);

            // Check if we have reached the end point
            if (x0 == x1 && y0 == y1) {
                break;
            }

            // Calculate error and adjust the current point
            int error2 = 2 * error;

            if (error2 > -deltaY) {
                error -= deltaY;
                x0 += signX;
            }
            if (error2 < deltaX) {
                error += deltaX;
                y0 += signY;
            }
        }
    }

    void Screen::set(const int x, const int y, const Common::Color color)
    {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return;

        buffer[y * width + x] = color;
    }

    Common::Color Screen::get(const int x, const int y) const {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return Common::Color::getWhite();

        return buffer[y * width + x];
    }

    int Screen::getHeight() const
    {
        return height;
    }

    int Screen::getWidth() const
    {
        return width;
    }

    std::vector<Common::Color> Screen::getBuffer()
    {
        return buffer;
    }

}
