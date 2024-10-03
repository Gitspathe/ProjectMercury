#ifndef CLIENT_H
#define CLIENT_H
#if SERVER
#include <chrono>
#include <SDL_net.h>

namespace Engine::Net
{
    class Client
    {
    protected:
        TCPsocket* socket;
        uint32_t uid = 0;
        float lastMsgTime = 0.0f;

    public:
        explicit Client(TCPsocket* socket, uint32_t uid)
            : socket(socket), uid(uid) {}

        TCPsocket* getSocket() const
        {
            return socket;
        }

        void receivedMsg()
        {
            lastMsgTime = 0.0f;
        }

        void incrementTime(const float deltaTime)
        {
            lastMsgTime += deltaTime;
        }

        float timeSinceLastMsg() const
        {
            return lastMsgTime;
        }
    };
}

#endif
#endif //CLIENT_H
