#include <cstdlib>
#include "GameObject.h"
#include "GameWorld.h"

namespace Engine::World
{
    bool GameWorld::init() { return true; }

    void GameWorld::update(const float deltaTime)
    {
        isLocked = true;
        for(const auto go : gameObjects) {
            go->update(deltaTime);
        }
        isLocked = false;

        for(const auto go : pendingToAdd) {
            gameObjects.insert(go);
        }
        for(const auto go : pendingToRemove) {
            gameObjects.erase(go);
        }
        pendingToAdd.clear();
        pendingToRemove.clear();
    }

    void GameWorld::render()
    {
        // TODO: Cameras.
    }

    void GameWorld::destroy()
    {
        for(const auto go : gameObjects) {
            go->destroy();
        }
        gameObjects.clear();
    }

    void GameWorld::register_gameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        if(isLocked) {
            pendingToAdd.insert(gameObject);
            pendingToRemove.erase(gameObject);
            return;
        }
        gameObjects.insert(gameObject);
    }

    void GameWorld::unregister_gameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        if(isLocked) {
            pendingToAdd.erase(gameObject);
            pendingToRemove.insert(gameObject);
            return;
        }
        gameObjects.erase(gameObject);
    }
}
