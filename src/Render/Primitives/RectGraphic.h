#ifndef RECTGRAPHIC_H
#define RECTGRAPHIC_H
#include "../../Common/RectF.h"
#include "../../Common/ColorRGBA.h"
#include "../../Common/ColorRGB.h"

namespace Render {
    class Surface;
}

namespace Render::Primitives
{
    struct RectGraphic
    {
    private:
        Common::RectF rect;
        Common::ColorRGBA color;

    public:
        RectGraphic(const Common::RectF &rect, const Common::ColorRGBA &color)
            : rect(rect), color(color) {}

        RectGraphic(const float x, const float y, const float w, const float h, const Common::ColorRGBA color)
        {
            rect = Common::RectF(x, y, w, h);
            this->color = color;
        }

        Common::RectF getShape() const
        {
            return rect;
        }

        Common::ColorRGBA getColorRGBA() const
        {
            return color;
        }

        Common::ColorRGB getColorRGB() const
        {
            return Common::ColorRGB(color.r, color.g, color.b);
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

        void draw(Surface& surface, bool centered = false) const;
    };
}

#endif //RECTGRAPHIC_H
