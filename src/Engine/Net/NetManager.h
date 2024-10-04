#ifndef NETMANAGER_H
#define NETMANAGER_H
#include <memory>
#include <string>
#include "../../Engine.h"
#include "../Core/Subsystem.h"
#include "Net.h"

namespace Engine::Net
{
    class NetHandler;
    class PacketManager;
    class Peer;

    class NetManager final : public Core::Subsystem
    {
    public:

        NetManager();
        ~NetManager() override;

        std::string getName() override
        {
            return "NetManager";
        }

        uint32_t getOrder() override
        {
            return Core::SubsystemExecOrder::NET_MANAGER;
        }

        PacketManager& getPacketManager() const;
        NetHandler& getNetHandler() const;
        std::unordered_map<PeerUID, std::shared_ptr<Peer>>& getPeers();
        bool tryRegisterPeer(const std::shared_ptr<Peer>& peer);
        bool tryUnregisterPeer(const std::shared_ptr<Peer>& peer);
        bool tryGetPeer(PeerUID uid, Peer& outPeer);
        bool connect(const std::string& endpoint) const;
        void disconnect() const;

    private:
        std::unique_ptr<PacketManager> packetManager;
        std::unique_ptr<NetHandler> handler;
        std::unordered_map<PeerUID, std::shared_ptr<Peer>> peers;

        bool onInit() override;
        void onDestroy() override;
        void onUpdate(float deltaTime) override;
    };
}

#endif //NETMANAGER_H
