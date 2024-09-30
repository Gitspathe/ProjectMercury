#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <list>
#include <SDL_events.h>

namespace Engine::Input
{
    class InputManager
    {
    public:
        static InputManager* getInstance() {
            return instance;
        }

        InputManager();

        static void update(float deltaTime);
        static void onKeyboardEvent(const SDL_KeyboardEvent &ev);
        static bool isKeyDown(SDL_Scancode key);
        static bool isKeyUp(SDL_Scancode key);
        static bool wasKeyPressedThisFrame(SDL_Scancode key);
        static bool wasKeyReleasedThisFrame(SDL_Scancode key);
    private:
        std::list<SDL_Scancode> keysDownThisFrame = std::list<SDL_Scancode>();
        std::list<SDL_Scancode> keysDownLastFrame = std::list<SDL_Scancode>();
        std::list<SDL_KeyboardEvent> keyboardEvents = std::list<SDL_KeyboardEvent>();

        static InputManager* instance;

        static void handleKeyboardEvent(const SDL_KeyboardEvent &ev);
    };
}

#endif //INPUTMANAGER_H
