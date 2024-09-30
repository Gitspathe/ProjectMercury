#ifndef RECTGRAPHIC_H
#define RECTGRAPHIC_H
#include "../../Common/RectF.h"
#include "../Surface.h"
#include "../../Common/ColorConversion.h"
#include "../../Common/Colors.h"

namespace Engine::Render::Primitives
{
    template<typename TSurface>
    void drawInternal(
        std::vector<TSurface>& buffer,
        const int startX,
        const int startY,
        const int endX,
        const int endY,
        const int bufferW,
        const Common::ColorRGBA color,
        const uint8_t blendMode = Common::BlendMode::COLOR)
    {
        const auto data = buffer.data();
        switch(blendMode) {
            case Common::BlendMode::CLEAR: {
                const auto col = Common::ColorConversion::getClear<TSurface>();
                for(int y = startY; y < endY; ++y) {
                    const int rowOffset = y * bufferW + startX;
                    std::fill(data + rowOffset, data + rowOffset + (endX - startX), col);
                }
            } break;
            case Common::BlendMode::COLOR: {
                const auto col = Common::ColorConversion::convertColor<TSurface>(color);
                for(int y = startY; y < endY; ++y) {
                    const int rowOffset = y * bufferW + startX;
                    std::fill(data + rowOffset, data + rowOffset + (endX - startX), col);
                }
            } break;
            case Common::BlendMode::ADD: {
                for(int y = startY; y < endY; ++y) {
                    for(int x = startX; x < endX; ++x) {
                        buffer[y * bufferW + x] = buffer[y * bufferW + x] + color;
                    }
                }
            } break;
            case Common::BlendMode::SUBTRACT: {
                for(int y = startY; y < endY; ++y) {
                    for(int x = startX; x < endX; ++x) {
                        buffer[y * bufferW + x] = buffer[y * bufferW + x] - color;
                    }
                }
            } break;
            case Common::BlendMode::MULTIPLY: {
                for(int y = startY; y < endY; ++y) {
                    for(int x = startX; x < endX; ++x) {
                        buffer[y * bufferW + x] = buffer[y * bufferW + x] * color;
                    }
                }
            } break;
            case Common::BlendMode::DIVIDE: {
                for(int y = startY; y < endY; ++y) {
                    for(int x = startX; x < endX; ++x) {
                        buffer[y * bufferW + x] = buffer[y * bufferW + x] / color;
                    }
                }
            } break;
        }
    }

    template<typename T>
    struct RectGraphic
    {
    private:
        Common::RectF rect;
        T color;
        uint8_t blendMode;

    public:
        RectGraphic(const Common::RectF& rect, const T color, const uint8_t blendMode = Common::BlendMode::COLOR)
            : rect(rect), color(color), blendMode(blendMode) {}

        RectGraphic(
            const float x,
            const float y,
            const float w,
            const float h,
            const T color,
            const uint8_t blendMode = Common::BlendMode::COLOR)
        {
            rect = Common::RectF(x, y, w, h);
            this->color = color;
            this->blendMode = blendMode;
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

            Common::ColorRGBA col = Common::ColorConversion::convertColor<Common::ColorRGBA>(color);
            drawInternal(buffer, startX, startY, endX, endY, sW, col, blendMode);
        }
    };
}

#endif //RECTGRAPHIC_H
