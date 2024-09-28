#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include <cmath>
#include "../Common/Color3.h"
#include "../Common/RectF.h"

namespace Render
{
    class Screen
    {
    private:
        std::vector<Common::Color3> buffer;
        int width, height;

    public:
        Screen(const int width, const int height)
        {
            this->width = width;
            this->height = height;
            buffer = std::vector<Common::Color3>(height * width);
            clear(Common::Color3::Black);
        }

        Common::Color3 get(int x, int y) const
        {
#ifndef NDEBUG
            if(x < 0 || x >= width || y < 0 || y >= height)
                return Common::Color3::Black;
#endif

            return buffer[y * width + x];
        }
        Common::Color3 getSafe(const int x, const int y) const
        {
            if(x < 0 || x >= width || y < 0 || y >= height)
                return Common::Color3::Black;

            return buffer[y * width + x];
        }

        void set(int x, int y, const Common::Color3 color)
        {
#ifndef NDEBUG
            if(x < 0 || x >= width || y < 0 || y >= height)
                return;
#endif

            buffer[y * width + x] = color;
        }

        void setSafe(const int x, const int y, const Common::Color3 color)
        {
            if(x < 0 || x >= width || y < 0 || y >= height)
                return;

            buffer[y * width + x] = color;
        }

        void clear(const Common::Color3 color)
        {
            // Reset the entire buffer with a fast memory copy.
            std::fill(buffer.begin(), buffer.end(), color);
        }

        void drawPoint(const int size, const int x, const int y, const Common::Color3 color, const bool centered = false)
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
            for(size_t x = startX; x < endX; ++x) {
                for(size_t y = startY; y < endY; ++y) {
                    setSafe(x, y, color);
                }
            }
        }

        void drawLine(int x0, int y0, const int x1, const int y1, const Common::Color3 color)
        {
            // Calculate deltas
            int deltaX = abs(x1 - x0);
            int deltaY = abs(y1 - y0);

            // Determine the direction of the line
            int signX = x0 < x1 ? 1 : -1;
            int signY = y0 < y1 ? 1 : -1;

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

        void drawRect(const Common::RectF rect, const Common::Color3 color, const bool centered = false)
        {
            int startX, startY, endX, endY;
            const int x1 = static_cast<int>(rect.getX());
            const int y1 = static_cast<int>(rect.getY());
            const int x2 = static_cast<int>(x1 + rect.getWidth());
            const int y2 = static_cast<int>(y1 + rect.getHeight());

            // Calculate coordinates based on whether the rectangle should be centered
            if(centered) {
                const int width = x2 - x1;
                const int height = y2 - y1;
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
            if(endX <= 0 || endY <= 0 || startX >= width || startY >= height)
                return;

            if(startX < 0) startX = 0;
            if(startY < 0) startY = 0;
            if(endX > width) endX = width;
            if(endY > height) endY = height;

            // Fill each row with a fast memory copy.
            const auto data = buffer.data();
            for(size_t y = startY; y < endY; ++y) {
                const int rowOffset = y * width + startX;
                std::fill(data + rowOffset, data + rowOffset + (endX - startX), color);
            }
        }

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }

        std::vector<Common::Color3>& getBuffer()
        {
            return buffer;
        }
    };
}

#endif //SCREEN_H
