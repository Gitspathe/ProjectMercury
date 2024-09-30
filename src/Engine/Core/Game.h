#ifndef GAME_H
#define GAME_H
#include <memory>
#include "../World/GameWorld.h"

namespace Engine::Render {
    class Screen;
}

namespace Engine::World {
    class GameWorld;
}

namespace Engine::Core
{
    class Game
    {
    protected:
        std::unique_ptr<World::GameWorld> world = nullptr;
        std::unique_ptr<Render::Screen> screen = nullptr;

        virtual void setup() {}
        virtual void onInit() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
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

        void init()
        {
            setup();
            onInit();
            world->init();
        }

        void update(const float deltaTime)
        {
            onUpdate(deltaTime);
            world->update(deltaTime);
        }

        void render()
        {
            onRender();
        }

        void destroy()
        {
            onDestroy();
            world->destroy();
        }
    };
}

#endif //GAME_H
