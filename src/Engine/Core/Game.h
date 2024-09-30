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
        std::unique_ptr<World::GameWorld> world = nullptr;
        std::unique_ptr<Render::Screen> screen = nullptr;
        std::vector<std::shared_ptr<ISubsystemSDLEventReceiver>> sdlEventReceivers = std::vector<std::shared_ptr<ISubsystemSDLEventReceiver>>();
        std::vector<std::shared_ptr<Subsystem>> subsystems = std::vector<std::shared_ptr<Subsystem>>();
        std::map<std::type_index, std::shared_ptr<Subsystem>> subsystemLookup = std::map<std::type_index, std::shared_ptr<Subsystem>>();
        bool isInit = false;

        virtual void setup() {}
        virtual void onInit() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRenderGame() {}
        virtual void onPostRenderGame() {}
        virtual void onDestroy() {}

    public:
        virtual ~Game()
        {
            world.reset();
            screen.reset();
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

        void init()
        {
            setup();
            onInit();
            world->init();
            isInit = true;
            std::sort(subsystems.begin(), subsystems.end(), [](const std::shared_ptr<Subsystem>& lhs, const std::shared_ptr<Subsystem>& rhs) {
                return lhs->getOrder() < rhs->getOrder();
            });
            for(const auto& subsystem : subsystems) {
                if(auto cast = std::dynamic_pointer_cast<ISubsystemSDLEventReceiver>(subsystem)) {
                    sdlEventReceivers.push_back(cast);
                }
                subsystem->init(shared_from_this());
            }
            for(const auto& subsystem : subsystems) {
                subsystem->postInit(shared_from_this());
            }
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
            onRenderGame();
            for(const auto& subsystem : subsystems) {
                subsystem->render();
            }
            onPostRenderGame();
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

        Render::Screen& getScreen() const
        {
            return *screen;
        }
    };
}

#endif //GAME_H
