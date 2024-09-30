#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector2.h"

namespace Engine::Common
{
    class Transform : public std::enable_shared_from_this<Transform>
    {
    private:
        Vector2 position;
        Vector2 scale;
        float angle;

    public:
        Transform(const Vector2 position, const Vector2 scale, const float angle)
            : position(position), scale(scale), angle(angle) {}

        Transform()
        {
            position = Vector2(0.0f, 0.0f);
            scale = Vector2(1.0f, 1.0f);
            angle = 0.0f;
        }

        Vector2 getPosition() const
        {
            return position;
        }

        Vector2 getScale() const
        {
            return scale;
        }

        float getAngle() const
        {
            return angle;
        }

        void setPosition(const Vector2 pos)
        {
            position = pos;
        }

        void setScale(const Vector2 scale)
        {
            this->scale = scale;
        }

        void setAngle(const float angle)
        {
            this->angle = angle;
        }

        void setParams(const Vector2 pos, const Vector2 scale, const float angle)
        {
            this->position = pos;
            this->scale = scale;
            this->angle = angle;
        }

        static std::shared_ptr<Transform> create(
            const Vector2 position = Vector2::Zero,
            const Vector2 scale = Vector2::One,
            const float angle = 0.0f)
        {
            return std::make_shared<Transform>(position, scale, angle);
        }
    };
}

#endif //TRANSFORM_H
