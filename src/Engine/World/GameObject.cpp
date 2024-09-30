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
            component->init();
        }
        if(isEnabled) {
            enableInternal();
        }
        on_init();
        isInit = true;
    }

    void GameObject::update(const float deltaTime)
    {
        if(isDestroyed)
            return;

        on_update(deltaTime);
        for(const auto& component : dynamicComponents) {
            component->update(deltaTime);
        }
    }

    void GameObject::destroy()
    {
        isDestroyed = true;
        on_destroy();
        world->unregister_gameObject(shared_from_this());
        if(isEnabled) {
            setEnabled(false);
        }
        for(const auto& component : components) {
            component->destroy();
        }
        components.clear();
    }

    void GameObject::enableInternal() const
    {
        for(const auto c : components) {
            c->setEnabled(true);
        }
    }

    void GameObject::disabledInternal() const
    {
        for(const auto c : components) {
            c->setEnabled(false);
        }
    }

}
