#ifndef NETHANDLER_H
#define NETHANDLER_H
#include <string>
#include "PacketManager.h"

namespace Engine::Net
{
    class Peer;
    class Packet;

    class NetHandler
    {
    protected:
        NetManager* netManager = nullptr;
        virtual bool onInit() = 0;
        virtual void onUpdate(float deltaTime) = 0;
        virtual bool onConnect(std::string& endpoint) = 0;
        virtual void onDisconnect() = 0;
        virtual void onShutdown() = 0;

    public:
        NetHandler() = default;
        virtual ~NetHandler() = default;

        virtual void send(Peer& peer, Packet& packet) = 0;

        bool init(NetManager* netManager)
        {
            this->netManager = netManager;
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
