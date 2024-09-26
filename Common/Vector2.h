#ifndef VECTOR2_H

namespace Common
{
    struct Vector2
    {
        float x, y;

        explicit Vector2(float x = 0.0f, float y = 0.0f);
        Vector2 operator+(const Vector2& rhs) const;
        Vector2 operator-(const Vector2& rhs) const;
        Vector2 operator*(const Vector2& rhs) const;
        Vector2 operator/(const Vector2& rhs) const;
    };
}

#define VECTOR2_H
#endif //VECTOR2_H
