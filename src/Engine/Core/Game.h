#ifndef GAME_H
#define GAME_H
#include <memory>
#include <algorithm>
#include <map>
#include <typeindex>
#include <vector>
#include "../World/GameWorld.h"
#include "Subsystem.h"
#include "../../Engine.h"

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
        bool isQuitting = false;

        virtual bool setup() { return true;}
        virtual bool onInit() { return true;}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onFinalizeRender() {}
        virtual void onQuit() {}
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
            world = std::make_unique<World::GameWorld>();
        }

        bool quitRequested() const
        {
            return isQuitting;
        }

        void addSubsystem(const std::shared_ptr<Subsystem> &subsystem)
        {
            if(isInit)
                throw std::runtime_error("Cannot add subsystems after initialization has been completed");

            if(getSubsystem(subsystem->getType()) != nullptr) {
                std::cerr << "Attempted to add duplicate subsystem '" << subsystem->getName() << "'" << std::endl;
                return;
            }
            subsystems.push_back(subsystem);
            subsystemLookup[subsystem->getType()] = subsystem;
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

        std::shared_ptr<Subsystem> getSubsystem(const std::type_index type)
        {
            auto it = subsystemLookup.find(type);
            if (it != subsystemLookup.end()){
                return std::dynamic_pointer_cast<Subsystem>(it->second);
            }
            return nullptr;
        }

        bool init()
        {
            std::string sep = "--------------------------------------------------";

            log::write << "\n" << sep << "\n\nExecute setup()" << log::endl;
            if(!setup()) {
                log::write << "setup() FAILURE" << log::endl;
                return false;
            }

            log::write << "\n" << sep << "\n\nExecute onInit()" << log::endl;
            if(!onInit()) {
                log::write << "onInit() FAILURE" << log::endl;
                return false;
            }

            log::write << "\n" << sep << "\n\nExecute world->init()" << log::endl;
            if(!world->init()) {
                log::write << "world->init() FAILURE" << log::endl;
                return false;
            }

            log::write << "\n" << sep << "\n\nDetermining subsystems..." << log::endl;

            isInit = true;
            std::sort(subsystems.begin(), subsystems.end(), [](const std::shared_ptr<Subsystem>& lhs, const std::shared_ptr<Subsystem>& rhs) {
                return lhs->getOrder() < rhs->getOrder();
            });

            log::write << "Subsystems registered:" << log::endl;
            for(const auto& subsystem : subsystems) {
                log::write << "  " << subsystem->getOrder() << ": " << subsystem->getName() << log::endl;
            }

            log::write << "\n\n" << sep << "\n\nInitializing subsystems..." << log::endl;
            for(const auto& subsystem : subsystems) {
                if(auto cast = std::dynamic_pointer_cast<ISubsystemSDLEventReceiver>(subsystem)) {
                    sdlEventReceivers.push_back(cast);
                }
                if(!subsystem->init(shared_from_this()))
                    return false;
            }

            log::write << "Subsystems initialized.\n\n" << sep << "\n\nExecuting postInit()..." << log::endl;
            for(const auto& subsystem : subsystems) {
                if(!subsystem->postInit(shared_from_this()))
                    return false;
            }

            log::write << "Subsystem postInit() complete.\n\n" << sep << "\n\nInitializing game..." << log::endl;
            return true;
#undef sep
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

        bool handleSDLEvent(SDL_Event& ev) const
        {
            if(ev.type == SDL_QUIT)
                return false;

            for(const auto& handler : sdlEventReceivers) {
                handler->handleEvent(ev);
            }
            return true;
        }

        void quit()
        {
            onQuit();
            isQuitting = true;
            // main.cpp will handle calling destroy() at the end of the frame.
        }

        void destroy()
        {
            log::flush();
            onDestroy();
            for(const auto& subsystem : subsystems) {
                subsystem->destroy();
            }
            world->destroy();
            subsystems.clear();
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
