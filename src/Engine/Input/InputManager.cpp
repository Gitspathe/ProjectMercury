#include "InputManager.h"
#include <list>
#include <SDL_events.h>
#include <stdexcept>
#include <algorithm>
#include <ostream>

namespace Input
{
    InputManager* InputManager::instance = nullptr;

    InputManager::InputManager()
    {
        if(instance != nullptr) {
            throw std::runtime_error("InputManager already initialized");
        }

        instance = this;
    }

    void InputManager::update(float deltaTime)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        inst->keysDownLastFrame.clear();
        std::copy(
            inst->keysDownThisFrame.begin(),
            inst->keysDownThisFrame.end(),
            std::back_inserter(inst->keysDownLastFrame)
        );

        // Handle keyboard events.
        for(const auto ev : inst->keyboardEvents) {
            handleKeyboardEvent(ev);
        }

        // Clean-up for next frame.
        inst->keyboardEvents.clear();
    }

    void InputManager::onKeyboardEvent(const SDL_KeyboardEvent& ev)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        inst->keyboardEvents.push_back(ev);
    }

    bool InputManager::isKeyDown(const SDL_Scancode key)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        return std::find(inst->keysDownThisFrame.begin(), inst->keysDownThisFrame.end(), key) != inst->keysDownThisFrame.end();
    }

    bool InputManager::isKeyUp(const SDL_Scancode key)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        return !isKeyDown(key);
    }

    bool InputManager::wasKeyPressedThisFrame(const SDL_Scancode key)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        bool pressedLastFrame = std::find(inst->keysDownLastFrame.begin(), inst->keysDownLastFrame.end(), key) != inst->keysDownLastFrame.end();
        bool pressedThisFrame = isKeyDown(key);
        return !pressedLastFrame && pressedThisFrame;
    }

    bool InputManager::wasKeyReleasedThisFrame(const SDL_Scancode key)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        bool pressedLastFrame = std::find(inst->keysDownLastFrame.begin(), inst->keysDownLastFrame.end(), key) != inst->keysDownLastFrame.end();
        bool pressedThisFrame = isKeyDown(key);
        return pressedLastFrame && !pressedThisFrame;
    }

    void InputManager::handleKeyboardEvent(const SDL_KeyboardEvent& ev)
    {
        InputManager* inst = getInstance();
        if(inst == nullptr)
            throw std::runtime_error("InputManager is not initialized");

        switch(ev.type) {
            case SDL_KEYDOWN: {
                const bool found = std::find(
                    inst->keysDownThisFrame.begin(),
                    inst->keysDownThisFrame.end(),
                    ev.keysym.scancode
                ) != inst->keysDownThisFrame.end();
                if(!found) {
                    inst->keysDownThisFrame.push_back(ev.keysym.scancode);
                }
            } break;
            case SDL_KEYUP: {
                inst->keysDownThisFrame.remove(ev.keysym.scancode);
            } break;
        }
    }
}
