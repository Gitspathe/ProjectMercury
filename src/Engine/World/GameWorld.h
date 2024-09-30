#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <unordered_set>
#include "GameWorld.h"

namespace World
{
    class GameObject;

    class GameWorld {
    protected:
        std::unordered_set<std::shared_ptr<GameObject>> gameObjects = std::unordered_set<std::shared_ptr<GameObject>>();
        std::unordered_set<std::shared_ptr<GameObject>> pendingToAdd = std::unordered_set<std::shared_ptr<GameObject>>();
        std::unordered_set<std::shared_ptr<GameObject>> pendingToRemove = std::unordered_set<std::shared_ptr<GameObject>>();
        bool isLocked = false;

    public:
        GameWorld() {}

        static std::unique_ptr<GameWorld> create()
        {
            return std::make_unique<GameWorld>();
        }

        void init();
        void update(float deltaTime);
        void render();
        void destroy();
        void register_gameObject(const std::shared_ptr<GameObject> &gameObject);
        void unregister_gameObject(const std::shared_ptr<GameObject> &gameObject);
    };
}
#endif //GAMEWORLD_H
