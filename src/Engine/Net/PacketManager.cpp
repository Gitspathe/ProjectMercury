#include "PacketManager.h"
#include "ByteStream.h"
#include "PacketHandler.h"
#include "Peer.h"

namespace Engine::Net
{
    PacketManager::PacketManager() = default;
    PacketManager::~PacketManager() = default;

    PacketHandler *PacketManager::getHandler(const uint8_t type)
    {
        if(handlers.find(type) == handlers.end()) {
            log::write << "No packet handler for ID '" << type << "' found" << log::endl;
            return nullptr;
        }
        return handlers[type].get();
    }

    Packet PacketManager::constructPacket(const uint8_t type, void *data)
    {
        if(handlers.find(type) == handlers.end()) {
            log::write << "No packet handler for ID '" << type << "' found" << log::endl;
            return Packet::Invalid;
        }
        return handlers[type].get()->construct(data);
    }

    void PacketManager::executePendingPackets()
    {
        for(const auto& peer : peerStreams) {
            std::vector<Packet*>& packets = peer.second->getPackets();
            for(Packet* packet : packets) {
                try {
                    executePacket(*packet);
                } catch(std::exception& e) {
                    log::write << "Packet execution failed: " << e.what() << log::endl;
                }
            }
            packets.clear();
        }
    }

    void PacketManager::executePacket(Packet &packet)
    {
        PacketHandler* handler = getHandler(packet.getType());
        if(handler == nullptr) {
            return;
        }
        handler->process(packet);
    }

    void PacketManager::onMessage(const Peer& peer, uint8_t *data, const uint16_t size)
    {
        if(peerStreams.find(peer.getUID()) == peerStreams.end()) {
            log::write << "No peer found for ID '" << peer.getUID() << "'" << log::endl;
            return;
        }
        peerStreams[peer.getUID()]->addBytes(data, size);
    }

    void PacketManager::addPeer(const Peer& peer)
    {
        if(peerStreams.find(peer.getUID()) != peerStreams.end()) {
            log::write << "Peer " << peer.getUID() << " already registered" << log::endl;
            return;
        }
        peerStreams[peer.getUID()] = std::make_unique<ByteStream>();
    }

    void PacketManager::removePeer(const Peer& peer)
    {
        peerStreams.erase(peer.getUID());
    }

}
