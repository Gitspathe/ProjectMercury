#include "RectF.h"

namespace Common
{
    RectF::RectF(float x1, float y1, float x2, float y2)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }

    const float RectF::getX1() const
    {
        return x1;
    }

    const float RectF::getY1() const
    {
        return y1;
    }

    const float RectF::getX2() const
    {
        return x2;
    }

    const float RectF::getY2() const
    {
        return y2;
    }

    void RectF::setX1(float x)
    {
        this->x1 = x;
    }

    void RectF::setY1(float y)
    {
        this->y1 = y;
    }

    void RectF::setX2(float x2)
    {
        this->x2 = x2;
    }

    void RectF::setY2(float y2)
    {
        this->y2 = y2;
    }
}
