#include "Vector2.h"

namespace Common
{
    Vector2::Vector2(const float x, const float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2 Vector2::operator+(const Vector2 &rhs) const
    {
        return Vector2(this->x + rhs.x, this->y + rhs.y);
    }

    Vector2 Vector2::operator-(const Vector2 &rhs) const
    {
        return Vector2(this->x - rhs.x, this->y - rhs.y);
    }

    Vector2 Vector2::operator*(const Vector2 &rhs) const
    {
        return Vector2(this->x * rhs.x, this->y * rhs.y);
    }

    Vector2 Vector2::operator/(const Vector2 &rhs) const
    {
        return Vector2(this->x / rhs.x, this->y / rhs.y);
    }
}
