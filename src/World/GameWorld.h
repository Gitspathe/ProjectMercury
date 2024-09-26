#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <list>

namespace World
{
    class GameObject;

    class GameWorld {
    public:
        GameWorld() = default;

        void init();
        void update(float deltaTime);
        void destroy();
        void register_gameObject(GameObject* gameObject);
        void unregister_gameObject(GameObject* gameObject);

        static GameWorld* getInstance();
    private:
        std::list<GameObject*> gameObjects;

        void on_init();
        void on_update(float deltaTime);
        void on_destroy();
    };
}
#endif //GAMEWORLD_H
