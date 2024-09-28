#include "RectGraphic.h"
#include "../Surface.h"
#include "../../Common/Color3.h"
#include "../../Common/Color4.h"
#include "../../Common/RectF.h"
using namespace Common;

namespace Render::Primitives
{
    void RectGraphic::draw(Surface& surface, const bool centered) const
    {
        int startX, startY, endX, endY;
        const int sW = surface.getWidth();
        const int sH = surface.getHeight();
        const int x1 = static_cast<int>(rect.getX());
        const int y1 = static_cast<int>(rect.getY());
        const int x2 = static_cast<int>(x1 + rect.getWidth());
        const int y2 = static_cast<int>(y1 + rect.getHeight());
        const int w = static_cast<int>(rect.getWidth());
        std::vector<Color3> buffer = surface.getBuffer();

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
        if(endX <= 0 || endY <= 0 || startX >= sW || startY >= sH)
            return;

        if(startX < 0) startX = 0;
        if(startY < 0) startY = 0;
        if(endX > sW) endX = sW;
        if(endY > sH) endY = sH;

        // Fill each row with a fast memory copy.
        const auto data = buffer.data();
        const auto col = Color3(color.r, color.g, color.b);
        for (int y = startY; y < endY; ++y) {
            const int rowOffset = y * w + startX;
            std::fill(data + rowOffset, data + rowOffset + (endX - startX), col);
        }
    }
}
