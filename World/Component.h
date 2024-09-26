#ifndef COMPONENT_H
#define COMPONENT_H

namespace World
{
    class GameObject;

    class Component
    {
    public:
        Component();
        ~Component();

        GameObject* getParent();

        void init(GameObject* parent);
        void update(float deltaTime);
        void destroy();
    private:
        GameObject* parent;

        void on_init();
        void on_update(float deltaTime);
        void on_destroy();
    };
}

#endif //COMPONENT_H
