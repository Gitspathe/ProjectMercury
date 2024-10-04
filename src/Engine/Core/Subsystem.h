#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H
#include <memory>
#include <SDL_events.h>

namespace Engine::Core
{
    class ISubsystemSDLEventReceiver
    {
    protected:
        ~ISubsystemSDLEventReceiver() = default;

    public:
        virtual void handleEvent(SDL_Event& ev) = 0;
    };

    class Game;
    class Subsystem
    {
    protected:
        std::weak_ptr<Game> game;
        virtual bool onInit() { return true; }
        virtual bool onPostInit() { return true; }
        virtual void onEarlyUpdate(float deltaTime) {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onLateUpdate(float deltaTime) {}
        virtual void onRenderGame() {}
        virtual void onPostRenderGame() {}
        virtual void onDestroy() {}

    public:
        Subsystem() {}
        virtual ~Subsystem() {}

        virtual std::string getName() = 0;
        virtual uint32_t getOrder() = 0;

        virtual const std::type_info& getType() const
        {
            return typeid(*this);
        }

        bool postInit(const std::shared_ptr<Game>& game)
        {
            this->game = game;
            return onPostInit();
        }

        bool init(const std::shared_ptr<Game>& game)
        {
            this->game = game;
            return onInit();
        }

        void earlyUpdate(const float deltaTime)
        {
            if (auto lock = game.lock()) {
                onEarlyUpdate(deltaTime);
            }
        }

        void update(const float deltaTime)
        {
            if (auto lock = game.lock()) {
                onUpdate(deltaTime);
            }
        }

        void lateUpdate(const float deltaTime)
        {
            if (auto lock = game.lock()) {
                onLateUpdate(deltaTime);
            }
        }

        void render()
        {
            if (auto lock = game.lock()) {
                onRenderGame();
            }
        }

        void postRender()
        {
            if (auto lock = game.lock()) {
                onPostRenderGame();
            }
        }

        void destroy()
        {
            onDestroy();
        }
    };
}

#endif //SUBSYSTEM_H
