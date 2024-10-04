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
        uint16_t size = 0;
        uint8_t* buffer = nullptr;

    public:
        Packet() = default;

        Packet(const uint32_t source, const uint8_t type, const uint16_t size, uint8_t* buffer)
            : source(source), type(type), size(size)
        {
            if(size > 0 && buffer != nullptr) {
                this->buffer = new uint8_t[size];
                std::memcpy(this->buffer, buffer, size);
            }
        }

        Packet(const Packet& copy)
            : source(copy.source), type(copy.type), size(copy.size)
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

        uint16_t getPayloadSize() const
        {
            return size;
        }

        uint8_t getHeaderSize() const
        {
            return sizeof(uint8_t) + sizeof(uint16_t);
        }

        uint16_t getFullSize() const
        {
            return getPayloadSize() + getHeaderSize();
        }

        uint8_t* getPayload() const
        {
            return buffer;
        }

        uint8_t* getData() const
        {
            // Allocate memory for the entire packet (header + payload)
            auto data = new uint8_t[getFullSize()];

            // Set the payload size in the first two bytes of the header (convert to network byte order)
            uint16_t payloadSizeNetworkOrder = ntohs(getPayloadSize()); // Convert to network byte order (big-endian)
            std::memcpy(data, &payloadSizeNetworkOrder, sizeof(payloadSizeNetworkOrder)); // Copy the size into the first 2 bytes

            // Set the packet type (assuming it's a single byte)
            data[2] = type;

            // Copy the payload data from the buffer into the packet (after the header)
            // Use the correct offset for the payload
            std::memcpy(data + getHeaderSize(), this->buffer, getPayloadSize());

            return data;
        }
    };

    inline const Packet Packet::Invalid = Packet(0, PacketTypes::INVALID, 0, nullptr);
}

#endif //PACKET_H
