#include "GameObject.h"
#include "Component.h"
#include "GameWorld.h"

namespace World
{
    void GameObject::init(const std::shared_ptr<GameWorld> &world)
    {
        this->world = world;
        setup();
        world->register_gameObject(shared_from_this());
        for(const auto& component : components) {
            component.second->init();
        }
        on_init();
    }

    void GameObject::update(const float deltaTime)
    {
        on_update(deltaTime);
        for(const auto& component : components) {
            component.second->update(deltaTime);
        }
    }

    void GameObject::destroy()
    {
        isDestroyed = true;
        on_destroy();
        world->unregister_gameObject(shared_from_this());
        for(const auto& component : components) {
            component.second->destroy();
        }
        components.clear();
    }
}
