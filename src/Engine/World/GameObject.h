#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <typeindex>
#include <vector>
#include "Component.h"
#include "../Common/Vector2.h"
#include "../Common/Transform.h"
#include "../World/TransformComponent.h"

namespace Engine::World
{
    class Component;
    class GameWorld;

    class GameObject : public std::enable_shared_from_this<GameObject>
    {
    protected:
        std::shared_ptr<GameWorld> world = nullptr;
        std::vector<std::shared_ptr<Component>> components = std::vector<std::shared_ptr<Component>>();
        std::vector<std::shared_ptr<Component>> dynamicComponents = std::vector<std::shared_ptr<Component>>();
        bool isInit = false;
        bool isDestroyed = false;
        bool isEnabled = true;

        virtual void on_init() {}
        virtual void setup() {}
        virtual void on_update(float deltaTime) {}
        virtual void onEnabled() {}
        virtual void onDisabled() {}
        virtual void on_destroy() {}

        void enableInternal() const;
        void disabledInternal() const;

    public:
        friend class Component;

        GameObject() {}

        template<typename T>
        static std::shared_ptr<T> create()
        {
            static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
            return std::make_shared<GameObject>();
        }

        template<typename T>
        static std::shared_ptr<T> createWithTransform(
            const Common::Vector2 pos = Common::Vector2::Zero,
            const Common::Vector2 scale = Common::Vector2::One,
            const float angle = 0.0f)
        {
            static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
            const std::shared_ptr<T> go = std::make_shared<T>();
            const std::shared_ptr<Common::Transform> tComp = *go->template addComponent<TransformComponent>();
            tComp->setParams(pos, scale, angle);
            return go;
        }

        bool getEnabled() const
        {
            return isEnabled;
        }

        void setEnabled(const bool val)
        {
            const bool oldState = this->isEnabled;
            this->isEnabled = val;
            if(isInit)
                return;

            if(val && !oldState) {
                enableInternal();
            } else if(!val && oldState) {
                disabledInternal();
            }
        }

        std::shared_ptr<GameWorld> getWorld() const
        {
            return world;
        }

        template<typename T>
        std::shared_ptr<T> addComponent()
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::addComponent called on destroyed GameObject");

            static_assert(std::is_base_of_v<Component, T>, "T must derive from World::Component");
            auto component = std::make_shared<T>();
            component->setGameObject(shared_from_this());
            components.push_back(component);
            component->setEnabled(true);
            return component;
        }

        template<typename T>
        void removeComponent(const std::shared_ptr<T>& component)
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::removeComponent called on destroyed GameObject");

            static_assert(std::is_base_of_v<Component, T>, "T must derive from World::Component");
            component->setEnabled(false);
            for(size_t i = 0; i < components.size(); i++) {
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
                const std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(c);
                if(typeid(std::shared_ptr<T>) == typeid(cast)) {
                    return cast;
                }
            }
            return nullptr;
        }

        template<typename T>
        bool hasComponent()
        {
            return getComponent<T>() != nullptr;
        }

        template<typename T>
        bool hasComponent(std::shared_ptr<T> component)
        {
            if(isDestroyed)
                return false;

            for(auto c : components) {
                const std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(c);
                if(cast.get() == component.get()) {
                    return true;
                }
            }
            return false;
        }

        void init(const std::shared_ptr<GameWorld> &world);
        void update(const float deltaTime);
        void destroy();
    };
}

#endif //GAMEOBJECT_H
