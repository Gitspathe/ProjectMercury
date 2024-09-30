#include <functional>
#include <SDL2/SDL.h>
#include "Engine/Input/InputManager.h"
#include "Game/TestGame.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

bool running = true;
bool isInit = false;
std::function<void()> loop;
Input::InputManager* inputManager;
TestGame* game;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void main_loop()
{
    loop();
}

void init()
{
    inputManager = new Input::InputManager();

    game = new TestGame();
    game->init();

    isInit = true;
}

void run()
{
    Input::InputManager::update(1.0f);
    game->update(0.1f);
    game->render();
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
                    Input::InputManager::onKeyboardEvent(e.key);
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
