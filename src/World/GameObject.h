#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <cstdint>
#include <map>
#include <memory>
#include <typeindex>

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
        bool isInit = false;
        bool isDestroyed = false;
        std::multimap<std::type_index, std::shared_ptr<Component>> components;

        virtual void on_init() {}
        virtual void setup() {}
        virtual void on_update(float deltaTime) {}
        virtual void on_destroy() {}

    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        template<typename T>
        static std::shared_ptr<T> create()
        {
            static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
            auto ptr = std::make_shared<GameObject>();
            ptr->components = std::multimap<std::type_index, std::shared_ptr<Component>>();
            return ptr;
        }

        std::shared_ptr<GameWorld> getWorld() const
        {
            return world;
        }

        virtual uint16_t GetType() const { return GameObjectTypes::NONE; }

        template<typename T>
        std::shared_ptr<T> addComponent()
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::addComponent called on destroyed GameObject");

            std::type_index key(typeid(T));
            auto component = std::make_shared<T>();
            component->setGameObject(shared_from_this());
            components.emplace(key, component);
            return component;
        }

        template<typename T>
        void removeComponent(T& component)
        {
            if(isDestroyed)
                throw std::runtime_error("GameObject::removeComponent called on destroyed GameObject");

            const std::type_index key(typeid(T));
            auto [fst, snd] = components.equal_range(key);
            for(auto it = fst; it != snd; ++it) {
                if (it->second.get() == &component) {
                    components.erase(it);
                    return;
                }
            }
        }

        template<typename T>
        std::shared_ptr<T>& getComponent()
        {
            if(isDestroyed)
                return nullptr;

            const std::type_index key(typeid(T));
            const auto it = components.find(key);
            return it != components.end() ? it->second.get() : nullptr;
        }

        void init(const std::shared_ptr<GameWorld> &world);
        void update(const float deltaTime);
        void destroy();
    };
}

#endif //GAMEOBJECT_H
