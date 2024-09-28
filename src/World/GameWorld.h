#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <unordered_set>
#include "GameWorld.h"

namespace World
{
    class GameWorld {
    protected:
        std::unordered_set<std::shared_ptr<GameObject>> gameObjects;
        std::unordered_set<std::shared_ptr<GameObject>> pendingToAdd;
        std::unordered_set<std::shared_ptr<GameObject>> pendingToRemove;
        bool isLocked = false;

        virtual void on_init() {}
        virtual void on_update(float deltaTime) {}
        virtual void on_destroy() {}

    public:
        GameWorld() = default;
        virtual ~GameWorld() = default;

        template<typename T>
        static std::shared_ptr<T> create()
        {
            static_assert(std::is_base_of_v<GameWorld, T>, "T must derive from GameWorld");
            auto ptr = std::make_shared<GameWorld>();
            ptr->gameObjects = std::unordered_set<std::shared_ptr<GameObject>>();
            ptr->pendingToAdd = std::unordered_set<std::shared_ptr<GameObject>>();
            ptr->pendingToRemove = std::unordered_set<std::shared_ptr<GameObject>>();
            return ptr;
        }

        void init();
        void update(float deltaTime);
        void destroy();
        void register_gameObject(const std::shared_ptr<GameObject> &gameObject);
        void unregister_gameObject(const std::shared_ptr<GameObject> &gameObject);
    };
}
#endif //GAMEWORLD_H
