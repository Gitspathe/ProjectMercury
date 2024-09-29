#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <vector>
#include "../Common/ColorRGB.h"

namespace Render
{

    class Surface : public std::enable_shared_from_this<Surface>
    {
    private:
        std::vector<Common::ColorRGB> buffer;
        int w, h;

    public:
        Surface(const int w, const int h)
        {
            this->w = w;
            this->h = h;
            buffer = std::vector<Common::ColorRGB>(h * w);
            clear(Common::ColorRGB::Black);
        }

        static std::shared_ptr<Surface> create(const int w, const int h)
        {
            return std::make_shared<Surface>(w, h);
        }

        std::vector<Common::ColorRGB>& getBuffer()
        {
            return buffer;
        }

        Common::ColorRGB getPixel(const int x, const int y) const
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return Common::ColorRGB::Black;
#endif

            return buffer[y * w + x];
        }

        Common::ColorRGB getPixelSafe(const int x, const int y) const
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return Common::ColorRGB::Black;

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

        void setPixel(const int x, const int y, const Common::ColorRGB color)
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;
#endif

            buffer[y * w + x] = color;
        }

        void setPixelSafe(const int x, const int y, const Common::ColorRGB color)
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;

            buffer[y * w + x] = color;
        }

        void clear(const Common::ColorRGB color)
        {
            // Reset the entire buffer with a fast memory copy.
            std::fill(buffer.begin(), buffer.end(), color);
        }

        void resize(const int w, const int h)
        {
            this->w = w <= 0 ? 1 : w;
            this->h = h <= 0 ? 1 : h;
            buffer.resize(this->h * this->w);
            clear(Common::ColorRGB::Black);
        }
    };

}

#endif //SURFACE_H
