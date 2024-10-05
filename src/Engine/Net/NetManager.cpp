#include "PacketManager.h"
#include "NetManager.h"
#include "NetHandler.h"
#include "HostNetHandler.h"
#include "ClientNetHandler.h"
#include "ServerNetHandler.h"
#include "TestPacketHandler.h"
#include "Peer.h"

namespace Engine::Net
{
    NetManager::NetManager()
        : packetManager(nullptr), handler(nullptr)
    {
        packetManager = std::make_unique<PacketManager>();
    }

    NetManager::~NetManager() = default;

    std::unordered_map<PeerUID, std::shared_ptr<Peer>>& NetManager::getPeers()
    {
        return peers;
    }

    std::unordered_map<PeerUID, uint16_t>& NetManager::getPeerSeqs()
    {
        return peerSeqs;
    }

    bool NetManager::tryRegisterPeer(const std::shared_ptr<Peer>& peer)
    {
        if(peers.find(peer->getUID()) == peers.end()) {
            peers[peer->getUID()] = peer;
            peerSeqs[peer->getUID()] = 1;
            packetManager->addPeer(*peer);
            return true;
        }
        return false;
    }

    bool NetManager::tryUnregisterPeer(const std::shared_ptr<Peer>& peer)
    {
        const bool ret = peers.erase(peer->getUID()) >= 1;
        if(ret) {
            peerSeqs.erase(peer->getUID());
            packetManager->removePeer(*peer);
        }
        return ret;
    }

    bool NetManager::tryGetPeer(const PeerUID uid, Peer& outPeer)
    {
        if(peers.find(uid) != peers.end()) {
            outPeer = *peers[uid];
            return true;
        }
        return false;
    }

    bool NetManager::tryGetPeerSeq(const PeerUID uid, uint16_t& outSeq)
    {
        if(peerSeqs.find(uid) != peerSeqs.end()) {
            outSeq = peerSeqs[uid];
            return true;
        }
        return false;
    }

    bool NetManager::tryIncrementSeq(const PeerUID uid, uint16_t& outSeq)
    {
        if(tryGetPeerSeq(uid, outSeq)) {
            peerSeqs[uid] += 1;
            return true;
        }
        return false;
    }

    bool NetManager::connect(const std::string &endpoint) const
    {
        if(handler == nullptr) {
            log::write << "Connection failed - NetHandler is NULL." << log::endl;
            return false;
        }
        if(!handler->connect(endpoint)) {
            log::write << "Connection failed." << log::endl;
            return false;
        }
        return true;
    }

    void NetManager::disconnect() const
    {
        if(handler == nullptr)
            return;

        handler->shutdown();
    }

    PacketManager& NetManager::getPacketManager() const
    {
        return *packetManager;
    }

    NetHandler & NetManager::getNetHandler() const
    {
        return *handler;
    }

    bool NetManager::onInit()
    {
#if CLIENT && !SERVER
        log::write << "Initializing NetHandler as a client." << log::endl;
        handler = std::make_unique<ClientNetHandler>();
#elif SERVER && !CLIENT
        log::write << "Initializing NetHandler as the server." << log::endl;
        handler = std::make_unique<ServerNetHandler>();
#else
        log::write << "Initializing NetHandler as the host." << log::endl;
        handler = std::make_unique<HostNetHandler>();
#endif

        if(handler == nullptr || !handler->init(this)) {
            log::write << "Failed to initialize NetManager" << log::endl;
            return false;
        }

        packetManager->registerHandler<TestPacketHandler>();
        return true;
    }

    void NetManager::onDestroy()
    {
        disconnect();
    }

    void NetManager::onUpdate(const float deltaTime)
    {
        if(handler != nullptr) {
            handler->update(deltaTime);
        }
        packetManager->executePendingPackets();
    }

}