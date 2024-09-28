#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <cstdint>
#include <memory>
#include <typeindex>
#include <vector>

#include "Component.h"

namespace World
{
    class Component;
    class GameWorld;

    namespace GameObjectTypes
    {
        constexpr uint16_t NONE = 0;
        constexpr uint16_t PLAYER = 1;
    }

    class GameObject : public std::enable_shared_from_this<GameObject>
    {
    protected:
        std::shared_ptr<GameWorld> world = nullptr;
        std::vector<std::shared_ptr<Component>> components;
        std::vector<std::shared_ptr<Component>> dynamicComponents;
        bool isInit = false;
        bool isDestroyed = false;

        virtual void on_init() {}
        virtual void setup() {}
        virtual void on_update(float deltaTime) {}
        virtual void on_destroy() {}

    public:
        GameObject()
        {
            components = std::vector<std::shared_ptr<Component>>();
            dynamicComponents = std::vector<std::shared_ptr<Component>>();
        }

        template<typename T>
        static std::shared_ptr<T> create()
        {
            static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
            return std::make_shared<GameObject>();
        }

        std::shared_ptr<GameWorld> getWorld() const
        {
            return world;
        }

        virtual uint16_t GetType() const
        {
            return GameObjectTypes::NONE;
        }

        template<typename T>
        std::shared_ptr<T> addComponent()
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::addComponent called on destroyed GameObject");

            auto component = std::make_shared<T>();
            component->setGameObject(shared_from_this());
            components.push_back(component);
            if(component->isDynamic()) {
                dynamicComponents.push_back(component);
            }
            return component;
        }

        template<typename T>
        void removeComponent(const std::shared_ptr<T>& component)
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::removeComponent called on destroyed GameObject");

            static_assert(std::is_base_of_v<World::Component, T>, "T must derive from World::Component");
            if(component->isDynamic()) {
                for(int i = 0; i < dynamicComponents.size(); i++) {
                    if(dynamicComponents[i] == component) {
                        dynamicComponents.erase(dynamicComponents.begin() + i);
                    }
                }
            }
            for(int i = 0; i < components.size(); i++) {
                if(components[i] == component) {
                    components.erase(components.begin() + i);
                    return;
                }
            }
        }

        template<typename T>
        std::shared_ptr<T> getComponent()
        {
            if(isDestroyed)
                return nullptr;

            for(auto c : components) {
                if(std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(c)) {
                    return cast;
                }
            }
            return nullptr;
        }

        void init(const std::shared_ptr<GameWorld> &world);
        void update(const float deltaTime);
        void destroy();
    };
}

#endif //GAMEOBJECT_H
