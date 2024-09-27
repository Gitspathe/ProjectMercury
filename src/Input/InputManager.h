#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SDL_events.h>

namespace Input
{
    class InputManager
    {
    public:
        static const InputManager* getInstance() {
            return instance;
        }

        InputManager();

        void init();
        void update(float deltaTime);
        void onKeyboardEvent(SDL_KeyboardEvent ev);
        bool isKeyDown(SDL_Scancode key) const;
        bool isKeyUp(SDL_Scancode key) const;
        bool wasKeyPressedThisFrame(SDL_Scancode key) const;
        bool wasKeyReleasedThisFrame(SDL_Scancode key) const;
    private:
        static InputManager* instance;
        void handleKeyboardEvent(SDL_KeyboardEvent ev);
    };
}

#endif //INPUTMANAGER_H
