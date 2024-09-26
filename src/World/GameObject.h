#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <list>

namespace World
{
    class GameWorld;
    class Component;

    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

        GameWorld* getWorld();

        void init(GameWorld* world);
        void update(float deltaTime);
        void destroy();
    private:
        GameWorld* world;
        std::list<Component*> components;

        void on_init();
        void setup();
        void on_update(float deltaTime);
        void on_destroy();
    };
}

#endif //GAMEOBJECT_H
