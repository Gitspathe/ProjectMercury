#ifndef VECTOR2_H

namespace Common
{
    struct Vector2
    {
        float x, y;

        explicit Vector2(const float x = 0.0f, const float y = 0.0f)
            : x(x), y(y) {}

        Vector2 operator+(const Vector2 rhs) const
        {
            return Vector2(this->x + rhs.x, this->y + rhs.y);
        }

        Vector2 operator-(const Vector2 rhs) const
        {
            return Vector2(this->x - rhs.x, this->y - rhs.y);
        }

        Vector2 operator*(const Vector2 rhs) const
        {
            return Vector2(this->x * rhs.x, this->y * rhs.y);
        }

        Vector2 operator/(const Vector2 rhs) const
        {
            return Vector2(this->x / rhs.x, this->y / rhs.y);
        }

        static const Vector2 Zero;
        static const Vector2 One;
    };
}

#define VECTOR2_H
#endif //VECTOR2_H
