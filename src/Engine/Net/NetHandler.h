#ifndef NETHANDLER_H
#define NETHANDLER_H
#include <string>

namespace Engine::Net
{
    class NetHandler
    {
    protected:
        virtual bool onInit() = 0;
        virtual void onUpdate(float deltaTime) = 0;
        virtual bool onConnect(std::string& endpoint) = 0;
        virtual void onDisconnect() = 0;
        virtual void onShutdown() = 0;

    public:
        NetHandler() = default;
        virtual ~NetHandler() = default;

        bool init()
        {
            return onInit();
        }

        void update(const float deltaTime)
        {
            onUpdate(deltaTime);
        }

        bool connect(std::string endpoint)
        {
            return onConnect(endpoint);
        }

        void disconnect()
        {
            onDisconnect();
        }

        void shutdown()
        {
            onShutdown();
        }
    };
}

#endif //NETHANDLER_H
