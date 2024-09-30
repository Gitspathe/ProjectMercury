#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <iostream>
#include <ostream>
#include "Component.h"
#include "../Common/Transform.h"

namespace Engine::World
{
    class TransformComponent final : public Component
    {
    protected:
        std::shared_ptr<Common::Transform> transform = nullptr;

        void onInit() override
        {
            std::cout << "Transform::on_init" << std::endl;
        }

        void onUpdate(float deltaTime) override
        {
            transform->setPosition(transform->getPosition() + Common::Vector2(10,10));
            std::cout << "Transform::on_update - X: " << transform->getPosition().x << ", Y: " << transform->getPosition().y << std::endl;
        }

        void onDestroy() override
        {
            std::cout << "Transform::on_destroy" << std::endl;
        }

    public:
        TransformComponent()
        {
            transform = Common::Transform::create();
        }

        ~TransformComponent() override
        {
            transform = nullptr;
        }

        bool isDynamic() const override
        {
            return true;
        }

        // ReSharper disable once CppNonExplicitConversionOperator
        // NOLINT(*-explicit-constructor)
        operator std::shared_ptr<Common::Transform>() const
        {
            return transform;
        }
    };
}

#endif //TRANSFORMCOMPONENT_H
