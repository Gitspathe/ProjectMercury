#ifndef NET_H
#define NET_H
#include <cstdint>

namespace Engine::Net
{
    typedef uint64_t PeerUID;
}

namespace Engine::Net::PacketTypes
{
    constexpr uint8_t INVALID = 0;
    constexpr uint8_t TEST_MSG = 1;
    constexpr uint8_t RPC = 2;
}

#endif //NET_H
