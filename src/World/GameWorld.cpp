#include <cstdlib>
#include "GameObject.h"
#include "GameWorld.h"

namespace World
{
    void GameWorld::init()
    {
        on_init();
    }

    void GameWorld::update(float deltaTime)
    {
        on_update(deltaTime);
        for(auto go : gameObjects) {
            go->update(deltaTime);
        }
    }

    void GameWorld::destroy()
    {
        on_destroy();
        for(auto go : gameObjects) {
            go->destroy();
        }
        gameObjects.clear();
    }

    void GameWorld::register_gameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        gameObjects.insert(gameObject);
    }

    void GameWorld::unregister_gameObject(const std::shared_ptr<GameObject> &gameObject)
    {
        gameObjects.erase(gameObject);
    }
}
