#ifndef RECT_H
#define RECT_H

namespace Common
{
    class RectF
    {
    public:
        RectF(float x1 = 0.0f, float y1 = 0.0f, float x2 = 1.0f, float y2 = 1.0f);
        const float getX1() const;
        const float getY1() const;
        const float getX2() const;
        const float getY2() const;
        void setX1(float x);
        void setY1(float y);
        void setX2(float x2);
        void setY2(float y2);
    private:
        float x1, y1, x2, y2;
    };
}

#endif //RECT_H
