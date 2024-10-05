#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <cstring>
#include "Net.h"

namespace Engine::Net
{
    inline uint16_t ntohs(uint16_t netshort) {
        return (netshort << 8) | (netshort >> 8);
    }

    inline uint16_t htons(uint16_t hostshort) {
        return (hostshort << 8) | (hostshort >> 8);
    }

    class Packet
    {
    private:

        // Assigned by the receiver from UID.
        uint32_t source = 0;

        // Serialized.
        uint8_t type = PacketTypes::INVALID;
        uint16_t seq = 0;
        uint16_t size = 0;
        uint8_t* buffer = nullptr;

    public:

        static constexpr uint8_t HEADER_SIZE = sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint16_t);

        Packet() = default;

        Packet(const uint32_t source, const uint8_t type, const uint16_t size, const uint8_t* buffer)
            : source(source), type(type), size(size)
        {
            if(size > 0 && buffer != nullptr) {
                this->buffer = new uint8_t[size];
                std::memcpy(this->buffer, buffer, size);
            }
        }

        Packet(const Packet& copy)
            : source(copy.source), type(copy.type), seq(copy.seq), size(copy.size)
        {
            if(copy.size > 0 && copy.buffer != nullptr) {
                this->buffer = new uint8_t[size];
                std::memcpy(this->buffer, copy.buffer, size);
            }
        }

        Packet& operator=(const Packet& copy)
        {
            if(this == &copy)
                return *this;

            delete[] buffer;
            source = copy.source;
            type = copy.type;
            seq = copy.seq;
            size = copy.size;
            buffer = nullptr;
            if(copy.size > 0 && copy.buffer != nullptr){
                buffer = new uint8_t[copy.size];
                std::memcpy(buffer, copy.buffer, copy.size);
            }
            return *this;
        }

        ~Packet()
        {
            delete[] buffer;
        }

        static const Packet Invalid;

        uint32_t getSource() const
        {
            return source;
        }

        uint8_t getType() const
        {
            return type;
        }

        uint16_t getSeq() const
        {
            return seq;
        }

        void setSeq(const uint16_t seq)
        {
            this->seq = seq;
        }

        uint16_t getPayloadSize() const
        {
            return size;
        }

        uint16_t getFullSize() const
        {
            return getPayloadSize() + HEADER_SIZE;
        }

        uint8_t* getPayload() const
        {
            return buffer;
        }

        void writeInto(uint8_t* buffer) const
        {
            if(getFullSize() > MAX_PACKET_SIZE)
                throw std::runtime_error("Packet is larger than " + std::to_string(MAX_PACKET_SIZE) + " bytes.");

            // Copy header.
            buffer[0] = type;
            uint16_t seqNetworkOrder = ntohs(getSeq());
            uint16_t payloadSizeNetworkOrder = ntohs(getPayloadSize());
            std::memcpy(buffer + 1, &seqNetworkOrder, sizeof(seqNetworkOrder));
            std::memcpy(buffer + 3, &payloadSizeNetworkOrder, sizeof(payloadSizeNetworkOrder));

            // Copy payload.
            std::memcpy(buffer + HEADER_SIZE, this->buffer, getPayloadSize());
        }
    };

    inline const Packet Packet::Invalid = Packet(0, PacketTypes::INVALID, 0, nullptr);
}

#endif //PACKET_H
