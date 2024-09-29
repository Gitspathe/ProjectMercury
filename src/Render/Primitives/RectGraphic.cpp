#include "RectGraphic.h"

namespace Render::Primitives
{
    template<>
    void drawInternal<Common::ColorRGB, Common::ColorRGB>(
        std::vector<Common::ColorRGB>& buffer,
        const int startX,
        const int startY,
        const int endX,
        const int endY,
        const int bufferW,
        const Common::ColorRGB color)
    {
        const auto data = buffer.data();
        const auto col = Common::ColorConversion::convertColor<Common::ColorRGB>(color);
        for (int y = startY; y < endY; ++y) {
            const int rowOffset = y * bufferW + startX;
            std::fill(data + rowOffset, data + rowOffset + (endX - startX), col);
        }
    }
}
