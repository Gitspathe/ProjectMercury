#ifndef CLIENTNETHANDLER_H
#define CLIENTNETHANDLER_H
#if CLIENT && !SERVER && __EMSCRIPTEN__

#include <string>
#include <iomanip>
#include <random>
#include <sstream>
#include "NetHandler.h"
#include "emscripten/websocket.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class ClientNetHandler final : public NetHandler
    {
    private:
        EMSCRIPTEN_WEBSOCKET_T ws = 0;
        bool isConnected = false;
        std::string serverEndpoint;
        uint32_t clientUUID; // Store the generated UUID

        // Generate a unique identifier (UUID) for the client
        uint32_t generateUUID()
        {
            return rand() % 10000;
        }

        static EM_BOOL onOpen(int eventType, const EmscriptenWebSocketOpenEvent* ev, void *userData)
        {
            auto* handler = static_cast<ClientNetHandler*>(userData);
            log::write << "Connected to server at " << handler->serverEndpoint << log::endl;

            // Generate and send UUID
            handler->isConnected = true;
            handler->clientUUID = handler->generateUUID();
            handler->sendMessage("Hello, server!\n"); // Send UUID to server

            return EM_FALSE;
        }

        static EM_BOOL onMessage(int eventType, const EmscriptenWebSocketMessageEvent* ev, void *userData)
        {
            auto* handler = static_cast<ClientNetHandler*>(userData);
            //if (ev->isText) {
                std::string message(reinterpret_cast<const char*>(ev->data), ev->numBytes);
                log::write << "Received message from server: " << message << log::endl;

                // Handle responses from server if needed
            //}
            return EM_FALSE;
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
            // Handle any periodic client tasks. WebSocket communication will be event-driven.
            // You can implement sending a test message periodically if needed
        }

        bool onConnect(std::string &endpoint) override
        {
            serverEndpoint = "ws://" + endpoint; // WebSocket endpoint (e.g., ws://localhost:8080)
            return onInit();
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
        ClientNetHandler()
        {
            serverEndpoint = "ws://localhost:8082"; // Replace with your server address
        }

        void sendMessage(const std::string &message) const
        {
            if(isConnected && ws) {
                emscripten_websocket_send_binary(ws, new char[5] {'h','e','l','l','o'}, 5);
                log::write << "Sent message: " << message << log::endl; // Log sent message
                return;
            }
            log::write << "Unable to send message, not connected to server." << log::endl;
        }
    };
}

#endif
#endif //CLIENTNETHANDLER_H
