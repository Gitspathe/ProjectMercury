#ifndef CLIENTNETHANDLER_H
#define CLIENTNETHANDLER_H
#if CLIENT && !SERVER && __EMSCRIPTEN__
#include <string>
#include <iomanip>
#include <random>
#include <sstream>
#include <algorithm>
#include "NetHandler.h"
#include "emscripten/websocket.h"
#include "TestPacketHandler.h"
#include "Peer.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class ClientNetHandler final : public NetHandler
    {
    private:
        EMSCRIPTEN_WEBSOCKET_T ws = 0;
        bool isConnected = false;
        std::string serverEndpoint;
        std::shared_ptr<Peer> serverPeer;
        uint8_t* packetBuffer;

        static EM_BOOL onOpen(int eventType, const EmscriptenWebSocketOpenEvent* ev, void *userData)
        {
            auto* handler = static_cast<ClientNetHandler*>(userData);
            log::write << "Connected to server at " << handler->serverEndpoint << log::endl;

            handler->serverPeer = std::make_shared<Peer>(1);
            handler->netManager->tryRegisterPeer(handler->serverPeer);
            handler->isConnected = true;

            std::string msg = "Hello from my new packet handler thing!";
            PacketHandler* p = handler->netManager->getPacketManager().getHandler<TestPacketHandler>(PacketTypes::TEST_MSG);
            Packet packet = p->construct(&msg);
            handler->send(*handler->serverPeer, packet);
            return EM_TRUE;
        }

        static EM_BOOL onMessage(int eventType, const EmscriptenWebSocketMessageEvent* ev, void *userData)
        {
            const auto* handler = static_cast<ClientNetHandler*>(userData);
            if(handler->serverPeer == nullptr) {
                log::write << "Somehow received message from server, but the client is disconnected." << log::endl;
                return EM_FALSE;
            }

            auto* data = static_cast<uint8_t*>(ev->data);
            uint32_t totalBytes = ev->numBytes;
            if(totalBytes <= MAX_PACKET_SIZE) {
                // Packet can be process in one chunk.
                handler->netManager->getPacketManager().onMessage(handler->serverPeer->getUID(), data, totalBytes);
            } else {
                // Packet must be split into multiple chunks.
                uint32_t bytesProcessed = 0;
                while(bytesProcessed < totalBytes) {
                    uint32_t chunkSize = std::min(static_cast<uint32_t>(MAX_PACKET_SIZE), totalBytes - bytesProcessed);
                    handler->netManager->getPacketManager().onMessage(handler->serverPeer->getUID(), data + bytesProcessed, chunkSize);
                    bytesProcessed += chunkSize;
                }
            }
            return EM_TRUE;
        }

        static EM_BOOL onClose(int eventType, const EmscriptenWebSocketCloseEvent* ev, void *userData)
        {
            auto* handler = static_cast<ClientNetHandler*>(userData);
            log::write << "Disconnected from server." << log::endl;
            handler->isConnected = false;
            return EM_TRUE;
        }

        static EM_BOOL onError(int eventType, const EmscriptenWebSocketErrorEvent* ev, void *userData)
        {
            log::write << "WebSocket error occurred: socket=" << ev->socket << " event=" << eventType << log::endl;
            return EM_TRUE;
        }

    protected:

        bool onInit() override
        {
            if(!emscripten_websocket_is_supported()) {
                log::write << "WebSockets are not supported in this environment." << log::endl;
                return false;
            }

            EmscriptenWebSocketCreateAttributes wsAttr;
            emscripten_websocket_init_create_attributes(&wsAttr);
            wsAttr.url = serverEndpoint.c_str();
            wsAttr.protocols = nullptr;
            wsAttr.createOnMainThread = true;
            ws = emscripten_websocket_new(&wsAttr);
            if(ws <= 0) {
                log::write << "Failed to create WebSocket." << log::endl;
                return false;
            }

            emscripten_runtime_keepalive_push();
            emscripten_websocket_set_onopen_callback(ws, this, onOpen);
            emscripten_websocket_set_onmessage_callback(ws, this, onMessage);
            emscripten_websocket_set_onclose_callback(ws, this, onClose);
            emscripten_websocket_set_onerror_callback(ws, this, onError);
            return true;
        }

        void onUpdate(float deltaTime) override
        {
            if(isConnected) {
                //for(int i = 0; i < 1024; i++) {
                    std::string msg = "Hello from my new packet handler thing!";
                    PacketHandler* p = netManager->getPacketManager().getHandler<TestPacketHandler>(PacketTypes::TEST_MSG);
                    Packet packet = p->construct(&msg);
                    send(*serverPeer, packet);
                //}
            }
        }

        bool onConnect(std::string &endpoint) override
        {
            // serverEndpoint = "wss://" + endpoint;
            // return onInit();
            return true;
        }

        void onDisconnect() override
        {
            if(isConnected && ws) {
                emscripten_websocket_close(ws, 1000, "Client disconnect");
                emscripten_websocket_delete(ws);
                ws = 0;
            }
        }

        void onShutdown() override
        {
            onDisconnect();
        }

    public:
        void send(Peer& peer, Packet& packet) override
        {
            if(packet.getFullSize() > MAX_PACKET_SIZE) {
                log::write << "Attempted to send a packet larger than " << std::to_string(MAX_PACKET_SIZE) << " bytes" << log::endl;
                return;
            }

            uint16_t seq = 0;
            if(!netManager->tryIncrementSeq(serverPeer->getUID(), seq)) {
                log::write << "Failed to get peer seq from server." << log::endl;
                return;
            }
            packet.setSeq(seq);
            packet.writeInto(packetBuffer);
            emscripten_websocket_send_binary(ws, packetBuffer, packet.getFullSize());
        }

        ClientNetHandler()
        {
            packetBuffer = new uint8_t[MAX_PACKET_SIZE];
            serverEndpoint = "wss://mercurialorder.com:8082";
        }
    };
}

#endif
#endif //CLIENTNETHANDLER_H
