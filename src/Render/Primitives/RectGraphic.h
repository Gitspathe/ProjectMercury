#ifndef RECTGRAPHIC_H
#define RECTGRAPHIC_H
#include "../../Common/RectF.h"
#include "../Surface.h"
#include "../../Common/ColorConversion.h"
#include "../../Common/Colors.h"

namespace Render::Primitives
{
    template<typename TSurface, typename TCol>
    void drawInternal(
        std::vector<TSurface>& buffer,
        const int startX,
        const int startY,
        const int endX,
        const int endY,
        const int bufferW,
        const TCol color)
    {
        const auto col = Common::ColorConversion::convertColor<TCol>(color);
        for (int y = startY; y < endY; ++y) {
            for(int x = startX; x < endX; ++x) {
                TSurface colSurface = buffer[y * bufferW + x];
                TSurface newColor = colSurface + color;
                buffer[y * bufferW + x] = newColor;
            }
        }
    }

    template<typename T>
    struct RectGraphic
    {
    private:
        Common::RectF rect;
        T color;

    public:
        RectGraphic(const Common::RectF& rect, const T color)
            : rect(rect), color(color) {}

        RectGraphic(const float x, const float y, const float w, const float h, const T color)
        {
            rect = Common::RectF(x, y, w, h);
            this->color = color;
        }

        Common::RectF getShape() const
        {
            return rect;
        }

        T getColor() const
        {
            return color;
        }

        void setShape(const Common::RectF& shape)
        {
            rect = shape;
        }

        void setColor(const T color)
        {
            this->color = color;
        }

        template<typename TSurface>
        void draw(Surface<TSurface>& surface, const bool centered = false)
        {
            int startX, startY, endX, endY;
            const int sW = surface.getWidth();
            const int sH = surface.getHeight();
            const int x1 = static_cast<int>(rect.getX());
            const int y1 = static_cast<int>(rect.getY());
            const int x2 = static_cast<int>(x1 + rect.getWidth());
            const int y2 = static_cast<int>(y1 + rect.getHeight());
            const int w = static_cast<int>(rect.getWidth());
            const int h = static_cast<int>(rect.getHeight());
            std::vector<TSurface>& buffer = surface.getBuffer();

            // Calculate coordinates based on whether the rectangle should be centered
            if(centered) {
                startX = x1 - w / 2;
                startY = y1 - h / 2;
                endX = x1 + w / 2;
                endY = y1 + h / 2;
            } else {
                startX = x1;
                startY = y1;
                endX = x2;
                endY = y2;
            }
            if(endX <= 0 || endY <= 0 || startX >= sW || startY >= sH)
                return;

            if(startX < 0) startX = 0;
            if(startY < 0) startY = 0;
            if(endX > sW) endX = sW;
            if(endY > sH) endY = sH;

            drawInternal(buffer, startX, startY, endX, endY, sW, color);
        }
    };
}

#endif //RECTGRAPHIC_H
