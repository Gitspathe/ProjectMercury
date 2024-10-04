#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H
#include <cstdint>
#include "Packet.h"

namespace Engine::Net
{
    class PacketHandler
    {
    public:
        PacketHandler() = default;
        virtual ~PacketHandler() = default;

        virtual uint8_t getType() = 0;
        virtual Packet construct(void* obj) = 0;
        virtual bool verify(Packet& packet) = 0;
        virtual void process(Packet& packet) = 0;
    };
}

#endif //PACKETHANDLER_H
