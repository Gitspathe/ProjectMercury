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
        curSize  = 0;
        nextType = PacketTypes::INVALID;
        nextSeq  = 0;
        nextSize = 0;
    }

    void ByteStream::resetPacketParsing()
    {
        std::fill(std::begin(tempHeader), std::end(tempHeader), 0);
        remainingBytes = 0;
        offset = 0;
    }

    bool ByteStream::validateSeq(const uint16_t packetSeq) const
    {
        // Assume that if cur_seq is about to wrap around, that a low packet seq is okay.
        if(curSeq > std::numeric_limits<uint16_t>::max() - SEQ_MAXSIZE_TOLERANCE && packetSeq < SEQ_MAXSIZE_TOLERANCE)
            return true;

        // Otherwise, the packet must be of a larger sequence num.
        return packetSeq > curSeq;
    }

    bool ByteStream::queuePacket()
    {
        if(nextSize == 0) {
            log::write << "Cannot generate packet - data is empty." << log::endl;
            return false;
        }
        if(nextSize > MAX_PACKET_SIZE - PACKET_HEADER_SIZE) {
            log::write << "Next packet size exceeds " << std::to_string(MAX_PACKET_SIZE) << " bytes." << log::endl;
            return false;
        }

        curSeq = nextSeq;
        auto* copy = new uint8_t[nextSize];
        std::memcpy(copy, buffer, nextSize);
        auto* packet = new Packet(0, nextType, nextSize, copy);
        packet->setSeq(nextSeq);
        packets.push_back(packet);
        reset();
        delete[] copy;
        return true;
    }

    void ByteStream::addBytes(const uint8_t* data, const uint16_t size)
    {
        if(size > MAX_PACKET_SIZE) {
            log::write << "Attempted to write more than " << std::to_string(MAX_PACKET_SIZE) << " bytes to a ByteStream at once - discarding" << log::endl;
            curSeq++; // This should never happen. Increment seq as a cope.
            return;
        }

        uint16_t remaining = size;
        uint16_t offset = 0;
        while(remaining > 0) {

            // Step 1: Read the header.
            if(nextSize == 0 && curSize < Packet::HEADER_SIZE) {

                // Fill the buffer with incoming data for the header.
                uint16_t bytesToCopy = std::min(static_cast<int>(remaining), static_cast<int>(sizeof(tempHeader) - curSize));
                std::memcpy(tempHeader + curSize, data + offset, bytesToCopy);

                curSize += bytesToCopy;
                remaining -= bytesToCopy;
                offset += bytesToCopy;

                // Not enough data for the header.
                if(curSize != Packet::HEADER_SIZE) {
                    continue;
                }

                // Fill the header.
                nextType = tempHeader[0];                                       // Type     (1 byte)
                nextSeq  = ntohs(*reinterpret_cast<uint16_t*>(tempHeader + 1)); // Sequence (2 bytes)
                nextSize = ntohs(*reinterpret_cast<uint16_t*>(tempHeader + 3)); // Size     (2 bytes)

                // Check if the packet is a duplicate (same sequence), or too large.
                bool seqValid    = validateSeq(nextSeq);
                bool withinLimit = nextSize <= MAX_PACKET_SIZE - PACKET_HEADER_SIZE;
                if(!seqValid || !withinLimit) {
                    if(!seqValid) {
                        log::write << "Detected duplicate packet, sequence is out-of-order - Discarding." << log::endl;
                    }
                    if(!withinLimit) {
                        log::write << "Packet size exceeds maximum allowed size - Discarding." << log::endl;
                    }

                    // Discard the packet and reset. Seq always needs to increment.
                    curSeq = nextSeq;
                    reset();
                    resetPacketParsing();

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

            // No new data to process.
            if(nextSize <= 0)
                continue;

            // Step 2: Read the payload.
            if(curSize < nextSize) {
                const uint16_t bytesToCopy = std::min(static_cast<int>(remaining), nextSize - curSize);
                std::memcpy(buffer + curSize, data + offset, bytesToCopy);

                curSize += bytesToCopy;
                remaining -= bytesToCopy;
                offset += bytesToCopy;

                // All data retrieved - queue the packet for execution.
                if (curSize == nextSize) {
                    queuePacket();
                    resetPacketParsing();
                }
            }
        }
    }

}
