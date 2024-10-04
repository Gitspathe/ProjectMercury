#include <memory>
#include <unordered_map>
#include "Peer.h"
#include <random>

namespace Engine::Net
{
    bool Peer::generateUID(uint64_t& outUID, std::unordered_map<PeerUID, std::shared_ptr<Peer>>& curPeers, const size_t attempts) {
        static std::random_device rd;
        static auto eng = std::mt19937_64(rd());
        for(size_t i = 0; i < attempts; i++) {
            std::uniform_int_distribution<uint64_t> dist;
            uint64_t uid = dist(eng);
            if(curPeers.find(uid) == curPeers.end()) {
                outUID = uid;
                return true;
            }
        }
        return false;
    }
}
