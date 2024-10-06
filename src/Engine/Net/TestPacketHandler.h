#ifndef TESTPACKETHANDLER_H
#define TESTPACKETHANDLER_H
#include <string>
#include "PacketHandler.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class TestPacketHandler : public PacketHandler
    {
    public:

        static size_t maxMsgSize()
        {
            return 255;
        }

        uint8_t getType() override
        {
            return PacketTypes::TEST_MSG;
        }

        bool verify(Packet& packet) override
        {
            return true;
        }

        Packet construct(void* obj) override
        {
            auto* str = static_cast<std::string*>(obj);
            if(str->size() > maxMsgSize()) {
                log::write << "Attempted to construct a message packet with size > 255 chars." << log::endl;
                return Packet::Invalid;
            }

            const auto size = static_cast<uint16_t>(str->size());
            const auto buffer = new uint8_t[size];
            std::memcpy(buffer, str->data(), size);
            //std::copy(str->begin(), str->end(), buffer);
            auto packet = Packet(0, PacketTypes::TEST_MSG, size, buffer);
            delete[] buffer;
            return packet;
        }

        void process(Packet& packet) override
        {
            const std::string utf8String(reinterpret_cast<char*>(packet.getPayload()), packet.getPayloadSize());
            //log::write << "Received string: " << utf8String << log::endl;
        }
    };
}

#endif //TESTPACKETHANDLER_H
