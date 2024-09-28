#ifndef SURFACE_H
#define SURFACE_H
#include <vector>
#include "../Common/Color3.h"

namespace Render
{

    class Surface
    {
    private:
        std::vector<Common::Color3> buffer;
        int w, h;

    public:
        Surface(const int w, const int h)
        {
            this->w = w;
            this->h = h;
            buffer = std::vector<Common::Color3>(h * w);
            clear(Common::Color3::Black);
        }

        std::vector<Common::Color3>& getBuffer()
        {
            return buffer;
        }

        Common::Color3 getPixel(const int x, const int y) const
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return Common::Color3::Black;
#endif

            return buffer[y * w + x];
        }

        Common::Color3 getPixelSafe(const int x, const int y) const
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return Common::Color3::Black;

            return buffer[y * w + x];
        }

        int getWidth() const
        {
            return w;
        }

        int getHeight() const
        {
            return h;
        }

        void setPixel(const int x, const int y, const Common::Color3 color)
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;
#endif

            buffer[y * w + x] = color;
        }

        void setPixelSafe(const int x, const int y, const Common::Color3 color)
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;

            buffer[y * w + x] = color;
        }

        void clear(const Common::Color3 color)
        {
            // Reset the entire buffer with a fast memory copy.
            std::fill(buffer.begin(), buffer.end(), color);
        }

        void resize(const int w, const int h)
        {
            this->w = w <= 0 ? 1 : w;
            this->h = h <= 0 ? 1 : h;
            buffer.resize(this->h * this->w);
            clear(Common::Color3::Black);
        }
    };

}

#endif //SURFACE_H
