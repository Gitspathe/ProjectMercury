#include "Component.h"

namespace World
{
    GameObject *Component::getParent()
    {
        return parent;
    }

    void Component::init(GameObject* parent)
    {
        this->parent = parent;
    }

    void Component::update(float deltaTime)
    {
        on_update(deltaTime);
    }

    void Component::destroy()
    {
        on_destroy();
    }

    void Component::on_init() { }
    void Component::on_update(float deltaTime) { }
    void Component::on_destroy() { }

}
