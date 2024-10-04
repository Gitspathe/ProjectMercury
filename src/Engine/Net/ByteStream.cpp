#include "ByteStream.h"
#include "Packet.h"
#include "../../Engine.h"

namespace Engine::Net
{
    ByteStream::ByteStream() : buffer(new uint8_t[MAX_PACKET_SIZE]) {}

    ByteStream::~ByteStream()
    {
        delete[] buffer;
        for(const auto packet : packets) {
            delete packet;
        }
    }

    void ByteStream::reset()
    {
        curSize = 0;
        pendingBytes = 0;
        nextType = PacketTypes::INVALID;
        nextSize = 0;
    }

    bool ByteStream::queuePacket()
    {
        if(nextSize == 0) {
            log::write << "Cannot generate packet - data is empty" << log::endl;
            return false;
        }

        auto* copy = new uint8_t[nextSize];
        std::copy(buffer, buffer + nextSize, copy);
        auto* packet = new Packet(0, nextType, nextSize, copy);
        packets.push_back(packet);
        reset();
        delete[] copy;
        return true;
    }

    void ByteStream::addBytes(uint8_t* data, const uint16_t size)
    {
        uint16_t remaining = size;
        uint16_t offset = 0;
        uint8_t tempHeader[3];
        while (remaining > 0) {

            // Step 1: Read the header.
            if (nextSize == 0) {
                if (curSize < sizeof(tempHeader)) {
                    // Fill the buffer with incoming data for the header.
                    uint16_t bytesToCopy = std::min(static_cast<int>(remaining), static_cast<int>(sizeof(tempHeader) - curSize));
                    std::copy(data + offset, data + offset + bytesToCopy, tempHeader + curSize);
                    curSize += bytesToCopy;
                    remaining -= bytesToCopy;
                    offset += bytesToCopy;

                    // Not enough data for the header, so exit for now.
                    if(curSize != sizeof(tempHeader)) {
                        continue;
                    }

                    // Fill in the header.
                    nextSize = ntohs(*reinterpret_cast<uint16_t*>(tempHeader)); // size (2 bytes)
                    nextType = tempHeader[2];                                   // type (1 byte)

                    // Check if the packet size exceeds the maximum allowed size
                    if (nextSize > MAX_PACKET_SIZE) {
                        log::write << "Packet size exceeds maximum allowed size. Discarding packet data." << log::endl;

                        // Discard the packet and reset
                        reset();
                        uint16_t bytesToDiscard = nextSize;
                        if (remaining < bytesToDiscard) {
                            bytesToDiscard = remaining;
                        }
                        offset += bytesToDiscard;
                        remaining -= bytesToDiscard;
                        nextSize = 0;
                        continue;
                    }

                    // Reset current size for the next read of packet data
                    curSize = 0;
                }
            }

            // No new data to process.
            if(nextSize <= 0)
                continue;

            // Step 2: If we have read the length prefix and need to read the actual data
            if(curSize < nextSize) {
                uint16_t bytesToCopy = std::min(static_cast<int>(remaining), nextSize - curSize);
                std::copy(data + offset, data + offset + bytesToCopy, buffer + curSize);
                curSize += bytesToCopy;
                remaining -= bytesToCopy;
                offset += bytesToCopy;

                // All data retrieved - queue the packet for execution.
                if (curSize == nextSize) {
                    queuePacket();
                    curSize = 0;
                    nextSize = 0;
                }
            }
        }
    }

}
