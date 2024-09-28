#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <iostream>
#include <ostream>

#include "Component.h"

namespace World
{
    class Transform : public Component
    {
    protected:
        float x = 0.0f;
        float y = 0.0f;
        float angle = 0.0f;

        void on_init() override
        {
            std::cout << "Transform::on_init" << std::endl;
        }

        void on_update(float deltaTime) override
        {
            std::cout << "Transform::on_update - X: " << x << " Y:" << y << " ANG:" << angle << std::endl;
        }

        void on_destroy() override
        {
            std::cout << "Transform::on_destroy" << std::endl;
        }

    public:

        uint16_t getType() const override
        {
            return ComponentTypes::TRANSFORM;
        }
    };
}

#endif //TRANSFORM_H
