#include "Component.h"
#include "GameObject.h"

namespace Engine::World
{
    void Component::enableInternal()
    {
        if(!isDynamic() || isRegistered)
            return;

        const auto go = gameObject.lock();
        if(!go)
            throw std::runtime_error("GameObject has expired");

        std::shared_ptr<Component> shared = nullptr;
        for(auto ptr : go->components) {
            if(ptr.get() == this) {
                shared = ptr;
                break;
            }
        }
        if(shared == nullptr)
            throw std::runtime_error("Component is not assigned to the GameObject");

        go->dynamicComponents.push_back(shared);
        isRegistered = true;
    }

    void Component::disableInternal()
    {
        if(!isDynamic() || !isRegistered)
            return;

        const auto go = gameObject.lock();
        if(!go)
            throw std::runtime_error("GameObject has expired");

        std::shared_ptr<Component> shared = nullptr;
        for(auto ptr : go->components) {
            if(ptr.get() == this) {
                shared = ptr;
                break;
            }
        }
        if(shared == nullptr)
            throw std::runtime_error("Component is not assigned to the GameObject");

        for(int i = 0; i < go->dynamicComponents.size(); i++) {
            if(go->dynamicComponents[i] == shared) {
                go->dynamicComponents.erase(go->dynamicComponents.begin() + i);
                isRegistered = false;
                break;
            }
        }
    }
}
