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

        uint8_t nextType = PacketTypes::INVALID;
        uint16_t nextSeq = 0;
        uint16_t nextSize = 0;

        uint16_t curSeq = 0;

        uint8_t tempHeader[PACKET_HEADER_SIZE] = {};
        uint16_t remainingBytes = 0;
        uint16_t offset = 0;

        void reset();
        void resetPacketParsing();
        bool validateSeq(uint16_t packetSeq) const;
        bool queuePacket();

    public:

        static constexpr uint16_t SEQ_MAXSIZE_TOLERANCE = 10;

        ByteStream();
        ~ByteStream();

        std::vector<Packet*>& getPackets()
        {
            return packets;
        }

        uint16_t getCurSeq() const
        {
            return curSeq;
        }

        void addBytes(uint8_t* data, uint16_t size);
    };
}

#endif //BYTESTREAM_H
