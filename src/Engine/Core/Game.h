#ifndef GAME_H
#define GAME_H
#include <memory>
#include <algorithm>
#include <map>
#include <typeindex>
#include "../World/GameWorld.h"
#include "Subsystem.h"

namespace Engine::Render {
    class Screen;
}

namespace Engine::World {
    class GameWorld;
}

namespace Engine::Core
{
    class Game : public std::enable_shared_from_this<Game>
    {
    protected:
#if CLIENT
        std::unique_ptr<Render::Screen> screen;
#endif

        std::unique_ptr<World::GameWorld> world;
        std::vector<std::shared_ptr<ISubsystemSDLEventReceiver>> sdlEventReceivers;
        std::vector<std::shared_ptr<Subsystem>> subsystems;
        std::map<std::type_index, std::shared_ptr<Subsystem>> subsystemLookup;
        bool isInit = false;

        virtual bool setup() { return true;}
        virtual bool onInit() { return true;}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onFinalizeRender() {}
        virtual void onDestroy() {}

    public:
        virtual ~Game()
        {
            world.reset();
#if CLIENT
            screen.reset();
#endif
        }

        Game()
        {
            world = World::GameWorld::create();
        }

        template<typename T>
        void addSubSystem(std::shared_ptr<Subsystem> subsystem)
        {
            if(isInit)
                throw std::runtime_error("Cannot add subsystems after initialization has been completed");

            subsystems.push_back(subsystem);
            subsystemLookup[typeid(T)] = subsystem;
        }

        template <typename T>
        std::shared_ptr<T> getSubsystem()
        {
            auto it = subsystemLookup.find(typeid(T));
            if (it != subsystemLookup.end()){
                return std::dynamic_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        bool init()
        {
            if(!setup() || !onInit() || !world->init())
                return false;

            isInit = true;
            std::sort(subsystems.begin(), subsystems.end(), [](const std::shared_ptr<Subsystem>& lhs, const std::shared_ptr<Subsystem>& rhs) {
                return lhs->getOrder() < rhs->getOrder();
            });
            for(const auto& subsystem : subsystems) {
                if(auto cast = std::dynamic_pointer_cast<ISubsystemSDLEventReceiver>(subsystem)) {
                    sdlEventReceivers.push_back(cast);
                }
                if(!subsystem->init(shared_from_this()))
                    return false;
            }
            for(const auto& subsystem : subsystems) {
                if(!subsystem->postInit(shared_from_this()))
                    return false;
            }
            return true;
        }

        void update(const float deltaTime)
        {
            onUpdate(deltaTime);
            for(const auto& subsystem : subsystems) {
                subsystem->earlyUpdate(deltaTime);
            }
            for(const auto& subsystem : subsystems) {
                subsystem->update(deltaTime);
            }
            world->update(deltaTime);
            for(const auto& subsystem : subsystems) {
                subsystem->lateUpdate(deltaTime);
            }
        }

        void render()
        {
            onRender();
            for(const auto& subsystem : subsystems) {
                subsystem->render();
            }
            onFinalizeRender();
            for(const auto& subsystem : subsystems) {
                subsystem->postRender();
            }
        }

        void handleSDLEvent(SDL_Event& ev)
        {
            for(const auto& handler : sdlEventReceivers) {
                handler->handleEvent(ev);
            }
        }

        void destroy()
        {
            onDestroy();
            for(const auto& subsystem : subsystems) {
                subsystem->destroy();
            }
            world->destroy();
        }

#if CLIENT
        Render::Screen& getScreen() const
        {
            return *screen;
        }
#endif
    };
}

#endif //GAME_H
