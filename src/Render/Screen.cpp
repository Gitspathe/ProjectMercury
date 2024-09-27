#include "Screen.h"
#include "../Common/Color3.h"
#include "../Common/RectF.h"
#include <cmath>

namespace Render
{
    Screen::Screen(const int width, const int height)
    {
        this->width = width;
        this->height = height;
        buffer = std::vector<Common::Color3>(height * width);
        clear(Common::Color3::getBlack());
    }

    void Screen::clear(const Common::Color3 color)
    {
        for(int i = 0; i < height * width; i++) {
            buffer[i] = color;
        }
    }

    void Screen::drawPoint(const int size, const int x, const int y, const Common::Color3 color, const bool centered)
    {
        if(size == 1) {
            setSafe(x, y, color);
            return;
        }
        int startX, startY, endX, endY;
        if(centered) {
            startX = x - size / 2;
            startY = y - size / 2;
            endX = x + size / 2;
            endY = y + size / 2;
        } else {
            startX = x;
            startY = y;
            endX = x + size;
            endY = y + size;
        }
        for(int x = startX; x < endX; ++x) {
            for(int y = startY; y < endY; ++y) {
                setSafe(x, y, color);
            }
        }
    }

    void Screen::drawLine(int x0, int y0, int x1, int y1, Common::Color3 color)
    {
        // Calculate deltas
        int deltaX = abs(x1 - x0);
        int deltaY = abs(y1 - y0);

        // Determine the direction of the line
        int signX = (x0 < x1) ? 1 : -1;
        int signY = (y0 < y1) ? 1 : -1;

        // Bresenham's algorithm variables
        int error = deltaX - deltaY;

        while(true) {
            // Set the pixel at the current point
            setSafe(x0, y0, color);

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

    void Screen::drawRect(const Common::RectF rect, const Common::Color3 color, const bool centered)
    {
        int startX, startY, endX, endY;
        int x1 = rect.getX1();
        int y1 = rect.getY1();
        int x2 = rect.getX2();
        int y2 = rect.getY2();

        // Ensure x1, x2 and y1, y2 are ordered correctly (x1, y1 is top-left, x2, y2 is bottom-right)
        if(x1 > x2) std::swap(x1, x2);
        if(y1 > y2) std::swap(y1, y2);

        // Calculate coordinates based on whether the rectangle should be centered
        if(centered) {
            int width = x2 - x1;
            int height = y2 - y1;
            startX = x1 - width / 2;
            startY = y1 - height / 2;
            endX = x1 + width / 2;
            endY = y1 + height / 2;
        } else {
            startX = x1;
            startY = y1;
            endX = x2;
            endY = y2;
        }

        for (int x = startX; x < endX; ++x) {
            for (int y = startY; y < endY; ++y) {
                setSafe(x, y, color);
            }
        }
    }

    /**
     * Blit a pixel to the screen. This is an unsafe function - in release builds
     * it does not perform bounds checking.
     *
     * @param x x coords
     * @param y y coords
     * @param color color
     */
    void Screen::set(const int x, const int y, const Common::Color3 color)
    {
#ifndef NDEBUG
        if(x < 0 || x >= width || y < 0 || y >= height)
            return;
#endif

        buffer[y * width + x] = color;
    }

    /**
     * A safer blit function which always bounds checks, even in release builds.
     *
     * @param x x coords
     * @param y y coords
     * @param color color
     */
    void Screen::setSafe(const int x, const int y, const Common::Color3 color)
    {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return;

        buffer[y * width + x] = color;
    }

    /**
     * Get a pixel from the screen. This is an unsafe function - in release builds
     * it does not perform bounds checking.
     *
     * @param x x coords
     * @param y y coords
     * @returns color at x,y coords.
     */
    Common::Color3 Screen::get(const int x, const int y) const {
#ifndef NDEBUG
        if(x < 0 || x >= width || y < 0 || y >= height)
            return Common::Color3::getBlack();
#endif

        return buffer[y * width + x];
    }

    /**
     * Get a pixel from the screen. This is always bounds-checked, even in release builds.
     *
     * @param x x coords
     * @param y y coords
     * @returns color at x,y coords, or black if out of bounds.
     */
    Common::Color3 Screen::getSafe(const int x, const int y) const
    {
        if(x < 0 || x >= width || y < 0 || y >= height)
            return Common::Color3::getBlack();

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

    std::vector<Common::Color3> Screen::getBuffer()
    {
        return buffer;
    }

}
