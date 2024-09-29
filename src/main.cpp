#include <iostream>
#include <functional>
#include <chrono>
#include <SDL2/SDL.h>
#include "Common/Color3.h"
#include "Input/InputManager.h"
#include "Render/OpenGLRenderer.h"
#include "Render/Screen.h"
#include "World/GameObject.h"
#include "World/GameWorld.h"
#include "World/TransformComponent.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::function<void()> loop;
bool running = true;
Render::OpenGLRenderer* renderer;
Render::Screen* screen;
Input::InputManager* inputManager;
bool isInit = false;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void main_loop()
{
    loop();
}

std::shared_ptr<World::GameObject> go;
std::shared_ptr<World::GameWorld> world;
void init()
{
    inputManager->init();
    world = World::GameWorld::create<World::GameWorld>();
    world->init();
    screen = new Render::Screen(512, 256);
    renderer = new Render::OpenGLRenderer();
    inputManager = new Input::InputManager();
    renderer->setRenderScale(2.0f);
    renderer->init(world, screen);
    isInit = true;

    world = World::GameWorld::create<World::GameWorld>();
    go = World::GameObject::create<World::GameObject>();
    std::shared_ptr<World::TransformComponent> transform = go->addComponent<World::TransformComponent>();

    std::shared_ptr<Common::Transform> testTrans = *transform;
    testTrans->setPosition(Common::Vector2(1,5));

    std::cout << "SIZE OF: " << sizeof(World::GameObject) << std::endl;

    go->init(world);
}

void run()
{
    world->update(0.1f);

    if(rand() % 100 < 50) {
        std::shared_ptr<World::TransformComponent> trans = go->getComponent<World::TransformComponent>();
        if(trans != nullptr) {
            go->removeComponent(trans);
        }
    }

    auto started = std::chrono::high_resolution_clock::now();
    inputManager->update(1.0f);
    screen->clear(Common::Color3::Black);
    renderer->update();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << std::endl;
}

int main()
{
    loop = [&] {
        if(!isInit) {
            init();
        }

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type){
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x, y; SDL_GetMouseState(&x, &y);
                    break;
                }
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    inputManager->onKeyboardEvent(e.key);
                }
                default:
                    break;
            }
        }
        run();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(running) main_loop();
#endif

    SDL_Quit();
    return 0;
}
