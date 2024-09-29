#ifndef RECTGRAPHIC_H
#define RECTGRAPHIC_H
#include "../../Common/RectF.h"
#include "../../Common/ColorRGBA.h"
#include "../../Common/ColorRGB.h"
#include "../Surface.h"
#include "../../Common/ColorConversion.h"

namespace Render::Primitives
{
    template<typename T>
    struct RectGraphic
    {
    private:
        Common::RectF rect;
        T color;

    public:
        RectGraphic(const Common::RectF rect, const T color)
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

        void setShape(const Common::RectF shape)
        {
            rect = shape;
        }

        void setColorRGBA(const Common::ColorRGBA color)
        {
            this->color = color;
        }

        void setColorRGB(const Common::ColorRGB color)
        {
            this->color = Common::ColorRGBA(color.r, color.g, color.b);
        }

        template<typename TSurface>
        void draw(Surface<TSurface>& surface, bool centered = false)
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

            // Fill each row with a fast memory copy.
            const auto data = buffer.data();
            const auto col = Common::ColorConversion::convertColor<TSurface>(color);
            for (int y = startY; y < endY; ++y) {
                const int rowOffset = y * sW + startX;
                std::fill(data + rowOffset, data + rowOffset + (endX - startX), col);
            }
        }
    };
}

#endif //RECTGRAPHIC_H
