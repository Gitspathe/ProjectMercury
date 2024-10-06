#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "PacketHandler.h"
#include "Net.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class Packet;
    class Peer;
    class ByteStream;
    class PacketHandler;

    class PacketManager
    {
    private:
        std::unordered_map<uint8_t, std::unique_ptr<PacketHandler>> handlers;
        std::unordered_map<PeerUID, std::unique_ptr<ByteStream>> peerStreams;

    public:

        PacketManager();
        ~PacketManager();

        template<typename T>
        bool registerHandler()
        {
            std::unique_ptr<PacketHandler> handler = std::make_unique<T>();
            if(handlers.find(handler->getType()) != handlers.end()) {
                log::write << "Attempted to register duplicate packet handler" << log::endl;
                return false;
            }
            handlers[handler->getType()] = std::move(handler);
            return true;
        }

        template<typename T>
        PacketHandler* getHandler(const uint8_t type)
        {
            const auto it = handlers.find(type);
            if(it == handlers.end()) {
                log::write << "No packet handler for ID '" << type << "' found" << log::endl;
                return nullptr;
            }
            return static_cast<T*>(it->second.get());
        }

        PacketHandler* getHandler(uint8_t type);
        Packet constructPacket(uint8_t type, void* data);
        void executePendingPackets();
        void executePacket(Packet& packet);
        void onMessage(const Peer& peer, uint8_t* data, uint16_t size);
        void addPeer(const Peer& peer);
        void removePeer(const Peer& peer);
    };
}

#endif //PACKETMANAGER_H
