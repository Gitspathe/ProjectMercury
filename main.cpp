#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::function<void()> loop;
bool running = true;

void main_loop()
{
    loop();
}

int main()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto* window = SDL_CreateWindow(
        "Project Mercury",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    SDL_GL_CreateContext(window);

    loop = [&] {
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
        glClearColor(0.f, 0x33 / 255.f, 0x66 / 255.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(running) main_loop();
#endif

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
