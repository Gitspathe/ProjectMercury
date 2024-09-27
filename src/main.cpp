#include <iostream>
#include <functional>
#include <chrono>
#include <SDL2/SDL.h>
#include "Common/Color3.h"
#include "Input/InputManager.h"
#include "Render/OpenGLRenderer.h"
#include "Render/Screen.h"
#include "World/GameWorld.h"

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

void init()
{
    inputManager->init();
    screen = new Render::Screen(512, 256);
    renderer = new Render::OpenGLRenderer();
    inputManager = new Input::InputManager();
    renderer->setRenderScale(2.0f);
    renderer->init(new World::GameWorld(), screen);
    isInit = true;
}

void run()
{
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
