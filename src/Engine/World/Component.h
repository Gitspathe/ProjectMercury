#ifndef COMPONENT_H
#define COMPONENT_H
#include <cstdint>
#include <memory>

namespace Engine::World
{
    class GameObject;

    class Component
    {
    protected:
        std::weak_ptr<GameObject> gameObject = std::weak_ptr<GameObject>();
        bool isInit = false;
        bool isEnabled = true;
        bool isRegistered = false;

        virtual void onInit() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onEnabled() {}
        virtual void onDisabled() {}
        virtual void onDestroy() {}

        void enableInternal();
        void disableInternal();

    public:
        Component() = default;
        virtual ~Component() = default;

        bool getEnabled() const
        {
            return isEnabled;
        }

        void setEnabled(const bool val)
        {
            const bool oldState = this->isEnabled;
            this->isEnabled = val;
            if(!isInit)
                return;

            if(val && !oldState) {
                enableInternal();
            } else if(!val && oldState) {
                disableInternal();
            }
        }

        virtual bool isDynamic() const
        {
            return true;
        }

        std::shared_ptr<GameObject> getGameObject() const
        {
            return gameObject.expired() ? nullptr : gameObject.lock();
        }

        void setGameObject(const std::shared_ptr<GameObject>& gameObject)
        {
            if(isInit)
                throw std::runtime_error("Component is already initialized");

            this->gameObject = gameObject;
        }

        void init()
        {
            onInit();
            isInit = true;
            if(isEnabled) {
                enableInternal();
            }
        }

        void update(const float deltaTime)
        {
            if(!isInit)
                return;

            onUpdate(deltaTime);
        }

        void destroy()
        {
            if(!isInit)
                return;

            if(isEnabled) {
                setEnabled(false);
            }
            onDestroy();
        }
    };
}

#endif //COMPONENT_H
