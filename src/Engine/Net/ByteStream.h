#ifndef BYTESTREAM_H
#define BYTESTREAM_H
#include <vector>
#include "Net.h"

namespace Engine::Net
{
    class Packet;
    class ByteStream
    {
        private:
        std::vector<Packet*> packets = std::vector<Packet*>();
        uint8_t* buffer = new uint8_t[MAX_PACKET_SIZE];
        uint16_t curSize = 0;
        uint16_t pendingBytes = 0;

        uint8_t nextType = PacketTypes::INVALID;
        uint16_t nextSize = 0;

        void reset();
        bool queuePacket();

    public:
        ByteStream();
        ~ByteStream();

        static constexpr uint16_t MAX_PACKET_SIZE = 1024;

        std::vector<Packet*>& getPackets()
        {
            return packets;
        }

        void addBytes(uint8_t* data, const uint16_t size);
    };
}

#endif //BYTESTREAM_H
