#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include "Render/OpenGLRenderer.h"
#include "Render/Screen.h"
#include "World/GameWorld.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::function<void()> loop;
bool running = true;
Render::OpenGLRenderer* renderer;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void main_loop()
{
    loop();
}

bool isInit = false;

void init()
{
    renderer = new Render::OpenGLRenderer();
    renderer->init(new World::GameWorld(), new Render::Screen(1024, 512));

    isInit = true;
}

void run()
{
    renderer->update();
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
                case SDL_QUIT:{
                    running = false;
                    break;
                }
                case SDL_MOUSEMOTION:{
                    int x, y; SDL_GetMouseState(&x, &y);
                    std::cout << x << ", " << y << std::endl;
                    break;
                }
                default:
                    break;
            }
        }
        run();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#endif

    init();

#ifndef __EMSCRIPTEN__
    while(running) main_loop();
#endif

    SDL_Quit();
    return 0;
}
