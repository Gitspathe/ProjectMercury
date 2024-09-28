#ifndef RECTGRAPHIC_H
#define RECTGRAPHIC_H
#include "../../Common/RectF.h"
#include "../../Common/Color4.h"
#include "../../Common/Color3.h"

namespace Render {
    class Surface;
}

namespace Render::Primitives
{
    struct RectGraphic
    {
    private:
        Common::RectF rect;
        Common::Color4 color;

    public:
        RectGraphic(const Common::RectF &rect, const Common::Color4 &color)
            : rect(rect), color(color) {}

        RectGraphic(const float x, const float y, const float w, const float h, Common::Color4 color)
        {
            rect = Common::RectF(x, y, w, h);
            this->color = color;
        }

        Common::RectF getShape() const
        {
            return rect;
        }

        Common::Color4 getColor4() const
        {
            return color;
        }

        Common::Color3 getColor3() const
        {
            return Common::Color3(color.r, color.g, color.b);
        }

        void setShape(Common::RectF shape)
        {
            rect = shape;
        }

        void setColor4(Common::Color4 color)
        {
            this->color = color;
        }

        void setColor3(Common::Color3 color)
        {
            this->color = Common::Color4(color.r, color.g, color.b);
        }

        void draw(Surface& surface, bool centered = false) const;
    };
}

#endif //RECTGRAPHIC_H
