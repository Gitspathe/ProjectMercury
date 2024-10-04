#ifndef CLIENT_H
#define CLIENT_H
#include <chrono>
#include <SDL_net.h>
#include "Net.h"

namespace Engine::Net
{
    class Peer
    {
    private:
#if SERVER
        TCPsocket socket;
#endif
        PeerUID uid = 0;
        float lastMsgTime = 0.0f;

    public:

#if SERVER
        explicit Peer(const TCPsocket socket, const PeerUID uid)
            : socket(socket), uid(uid) {}
#else
        explicit Peer(const uint32_t uid)
            : uid(uid) {}
#endif

#if SERVER
        TCPsocket getSocket() const
        {
            return socket;
        }
#endif

        bool isServer() const
        {
            return uid == 1;
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

        PeerUID getUID() const
        {
            return uid;
        }

        static bool generateUID(uint64_t& outUID, std::unordered_map<PeerUID, std::shared_ptr<Peer>>& curPeers, const size_t attempts);
    };
}

#endif
