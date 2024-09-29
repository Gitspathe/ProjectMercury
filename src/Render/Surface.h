#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <vector>
#include "../Common/ColorConversion.h"

namespace Render
{

    template<typename T>
    class Surface
    {
    private:
        std::vector<T> buffer;
        int w, h;

    public:
        Surface(const int w, const int h)
        {
            this->w = w;
            this->h = h;
            buffer = std::vector<T>(h * w);
            clear(T());
        }

        static std::shared_ptr<Surface> create(const int w, const int h)
        {
            return std::make_shared<Surface>(w, h);
        }

        std::vector<T>& getBuffer()
        {
            return buffer;
        }

        T getPixel(const int x, const int y) const
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return T();
#endif

            return buffer[y * w + x];
        }

        T getPixelSafe(const int x, const int y) const
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return T();

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

        template<typename TCol>
        void setPixel(const int x, const int y, const TCol& color)
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;
#endif

            buffer[y * w + x] = convertColor(color);
        }

        template<typename TCol>
        void setPixelSafe(const int x, const int y, const TCol& color)
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;

            buffer[y * w + x] = convertColor(color);
        }

        void clear(const T color)
        {
            // Reset the entire buffer with a fast memory copy.
            std::fill(buffer.begin(), buffer.end(), color);
        }

        void resize(const int w, const int h)
        {
            this->w = w <= 0 ? 1 : w;
            this->h = h <= 0 ? 1 : h;
            buffer.resize(this->h * this->w);
            clear(T());
        }
    };

}

#endif //SURFACE_H
