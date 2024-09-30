#ifndef RECT_H
#define RECT_H
#include "Vector2.h"

namespace Engine::Common
{
    class RectF
    {
    private:
        float x, y, w, h;

    public:
        explicit RectF(const float x = 0.0f, const float y = 0.0f, const float w = 1.0f, const float h = 1.0f)
            : x(x), y(y)
        {
            this-> w = w <= 0.0f ? 0.0f : w;
            this-> h = h <= 0.0f ? 0.0f : h;
        }

        float getX() const
        {
            return x;
        }

        float getY() const
        {
            return y;
        }

        float getWidth() const
        {
            return w;
        }

        float getHeight() const
        {
            return h;
        }

        Vector2	getCenter() const
        {
            return Vector2(x + w / 2.0f, y + h / 2.0f);
        }

        Vector2 getPosition() const
        {
            return Vector2(x, y);
        }

        Vector2 getSize() const
        {
            return Vector2(w, h);
        }

        void setPosition(const Vector2 position)
        {
            x = position.x;
            y = position.y;
        }

        void setSize(const Vector2 size)
        {
            w = size.x <= 0.0f ? 0.0f : size.x;
            h = size.y <= 0.0f ? 0.0f : size.y;
        }
    };
}

#endif //RECT_H
