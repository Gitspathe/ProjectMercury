#include <iostream>
#include <functional>
#include <chrono>
#include <SDL2/SDL.h>
#include "Common/ColorRGB.h"
#include "Input/InputManager.h"
#include "Render/OpenGLRenderer.h"
#include "Render/Screen.h"
#include "Render/Surface.h"
#include "Render/Primitives/RectGraphic.h"
#include "World/GameObject.h"
#include "World/GameWorld.h"
#include "World/TransformComponent.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::function<void()> loop;
bool running = true;
Render::OpenGLRenderer<Common::ColorRGBA>* renderer;
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
std::shared_ptr<Render::Surface<Common::ColorRGBA>> surface;
void init()
{
    world = World::GameWorld::create<World::GameWorld>();
    world->init();
    screen = new Render::Screen(512, 256);
    renderer = new Render::OpenGLRenderer<Common::ColorRGBA>();
    inputManager = new Input::InputManager();
    surface = Render::Surface<Common::ColorRGBA>::create(screen->getWidth(), screen->getHeight());
    renderer->setRenderScale(2.0f);
    renderer->init(surface);
    isInit = true;

    world = World::GameWorld::create<World::GameWorld>();
    go = World::GameObject::createWithTransform<World::GameObject>();
    go->init(world);

    std::cout << "SIZE OF: " << sizeof(World::GameObject) << std::endl;
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
    surface->clear(Common::ColorRGBA::Black);

    int size = 32;
    for(size_t i = 0; i < 100000; i++) {
        int w = screen->getWidth();
        int h = screen->getHeight();
        auto rectG = Render::Primitives::RectGraphic(
            Common::RectF((rand() % w) - size / 2, (rand() % h) - size / 2, size, size),
            Common::ColorRGB(rand() % 255, rand() % 255, rand() % 255)
        );
        rectG.draw(*surface);
    }

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
