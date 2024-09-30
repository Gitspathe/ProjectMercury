#ifndef SURFACE_H
#define SURFACE_H
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
            clear(Common::ColorConversion::getClear<T>());
        }

        std::vector<T>& getBuffer()
        {
            return buffer;
        }

        template<typename TCol>
        TCol getPixel(const int x, const int y) const
        {
#ifndef NDEBUG
            if(x < 0 || x >= w || y < 0 || y >= h)
                return T();
#endif

            return Common::ColorConversion::convertColor<TCol>(buffer[y * w + x]);
        }

        template<typename TCol>
        TCol getPixelSafe(const int x, const int y) const
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return T();

            return Common::ColorConversion::convertColor<TCol>(buffer[y * w + x]);
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

            buffer[y * w + x] = Common::ColorConversion::convertColor<T>(color);
        }

        template<typename TCol>
        void setPixelSafe(const int x, const int y, const TCol& color)
        {
            if(x < 0 || x >= w || y < 0 || y >= h)
                return;

            buffer[y * w + x] = Common::ColorConversion::convertColor<T>(color);
        }

        template<typename TCol>
        void clear(const TCol color)
        {
            // Reset the entire buffer with a fast memory copy.
            std::fill(buffer.begin(), buffer.end(), Common::ColorConversion::convertColor<T>(color));
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
