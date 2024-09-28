#ifndef COMPONENT_H
#define COMPONENT_H
#include <cstdint>

namespace World
{
    class GameObject;

    namespace ComponentTypes
    {
        constexpr uint16_t NONE = 0;
        constexpr uint16_t TRANSFORM = 1;
        constexpr uint16_t ENTITY = 2;
    }

    class Component
    {
    protected:
        std::weak_ptr<GameObject> gameObject = std::weak_ptr<GameObject>();
        bool isInit = false;

        virtual void on_init() = 0;
        virtual void on_update(float deltaTime) = 0;
        virtual void on_destroy() = 0;

    public:
        virtual ~Component() = default;

        std::shared_ptr<GameObject> getGameObject() const
        {
            return gameObject.expired() ? nullptr : gameObject.lock();
        }

        virtual uint16_t getType() const = 0;

        void setGameObject(const std::shared_ptr<GameObject>& gameObject)
        {
            if(isInit)
                throw std::runtime_error("Component is already initialized");

            this->gameObject = gameObject;
        }

        void init()
        {
            on_init();
            isInit = true;
        }

        void update(const float deltaTime)
        {
            if(!isInit)
                return;

            on_update(deltaTime);
        }

        void destroy()
        {
            if(!isInit)
                return;

            on_destroy();
        }
    };
}

#endif //COMPONENT_H
