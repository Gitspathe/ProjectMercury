#include <cstdlib>
#include "GameObject.h"
#include "GameWorld.h"

namespace World {
    void GameWorld::init()
    {
        on_init();
    }

    void GameWorld::update(float deltaTime)
    {
        on_update(deltaTime);
    }

    void GameWorld::destroy()
    {
        on_destroy();
    }

    void GameWorld::register_gameObject(GameObject* gameObject)
    {
        gameObjects.push_back(gameObject);
    }

    void GameWorld::unregister_gameObject(GameObject* gameObject)
    {
        gameObjects.remove(gameObject);
    }

    void GameWorld::on_init()
    {

    }

    void GameWorld::on_update(float deltaTime)
    {
        for(auto go : gameObjects) {
            go->update(deltaTime);
        }
    }

    void GameWorld::on_destroy()
    {
        for(auto go : gameObjects) {
            go->destroy();
            free(go);
        }
    }
}
