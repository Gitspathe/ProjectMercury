#include <cstdlib>
#include "Component.h"
#include "GameWorld.h"
#include "GameObject.h"

namespace World {
    GameObject::GameObject()  { }
    GameObject::~GameObject() { }

    GameWorld* world;

    GameWorld* GameObject::getWorld()
    {
        return world;
    }

    void GameObject::init(GameWorld* world)
    {
        components = std::list<Component*>();
        this->world = world;
        setup();
        world->register_gameObject(this);
        for(auto const component : components) {
            component->init(this);
        }
        on_init();
    }

    void GameObject::update(float deltaTime)
    {
        on_update(deltaTime);
        for(auto const component : components) {
            component->update(deltaTime);
        }
    }

    void GameObject::destroy()
    {
        on_destroy();
        world->unregister_gameObject(this);
        for(auto const component : components) {
            component->destroy();
            free(component);
        }
    }

    void GameObject::setup() { }
    void GameObject::on_init() { }
    void GameObject::on_update(const float deltaTime) { }
    void GameObject::on_destroy() { }

}
