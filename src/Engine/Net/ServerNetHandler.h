#ifndef SERVERNETHANDLER_H
#define SERVERNETHANDLER_H
#if SERVER && !CLIENT
#include "NetHandler.h"
#include "SDL2/SDL_net.h"
#include "Client.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class ServerNetHandler final : public NetHandler
    {
    protected:
        IPaddress ipAddr{};
        TCPsocket serverSocket = nullptr;
        std::vector<Client> clients = std::vector<Client>();
        char* buffer;
        SDLNet_SocketSet socketSet = nullptr;

        const float timeoutPeriod = 10.0f;

        bool onInit() override
        {
            if(SDLNet_Init() == -1) {
                log::write << "SDLNet_Init failed: " << SDLNet_GetError() << log::endl;
                SDLNet_Quit();
                return false;
            }
            if(SDLNet_ResolveHost(&ipAddr, nullptr, 8081) == -1) {
                log::write << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << log::endl;
                SDLNet_Quit();
                return false;
            }

            serverSocket = SDLNet_TCP_Open(&ipAddr);
            if(!serverSocket) {
                log::write << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << log::endl;
                SDLNet_Quit();
                return false;
            }

            socketSet = SDLNet_AllocSocketSet(32);
            if(!socketSet) {
                log::write << "Failed to create socket set: " << SDLNet_GetError() << log::endl;
                SDLNet_Quit();
                return false;
            }

            SDLNet_TCP_AddSocket(socketSet, serverSocket);

            log::write << "Server listening on port 8081" << log::endl;
            return true;
        }

        void onUpdate(const float deltaTime) override
        {
            if(SDLNet_CheckSockets(socketSet, 0) <= 0) {
                return;
            }

            if(SDLNet_SocketReady(serverSocket)) {
                if(TCPsocket newClientSocket = SDLNet_TCP_Accept(serverSocket)) {
                    IPaddress* addr = SDLNet_TCP_GetPeerAddress(newClientSocket);

                    Uint32 ipaddr;
                    ipaddr = SDL_SwapBE32(addr->host);
                    printf("Accepted a connection from %d.%d.%d.%d port %hu\n", ipaddr >> 24,
                           (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff,
                           addr->port);

                    log::write << "New client connected! host: " << addr->host << " port:" << addr->port << log::endl;
                    if(SDLNet_TCP_AddSocket(socketSet, newClientSocket) == -1) {
                        log::write << "Failed to add client socket to set: " << SDLNet_GetError() << log::endl;
                        SDLNet_TCP_Close(newClientSocket);
                    } else {
                        SDLNet_TCP_Send(newClientSocket, "aaa\n", 4);
                        clients.push_back(Client(&newClientSocket, rand() % 1000));  // Add the new client to the client list
                    }
                }
            }

            for(size_t i = 0; i < clients.size(); ++i) {

                SDLNet_TCP_Send(*clients[i].getSocket(), "bbb\n", 4);

                if(Client& client = clients[i]; SDLNet_SocketReady(client.getSocket())) {
                    int received = SDLNet_TCP_Recv(*client.getSocket(), buffer, 1024);
                    if(received > 0) {
                        std::string message(buffer, received);
                        log::write << "Received message from client: " << message << log::endl;

                        // Respond with a confirmation or handle the message appropriately
                        //std::string responseMessage = "Received your message: " + message;
                        //SDLNet_TCP_Send(*client.getSocket(), responseMessage.c_str(), responseMessage.size());

                    } else if (received == 0) {
                        // Client disconnected
                        log::write << "Client disconnected" << log::endl;
                        SDLNet_TCP_DelSocket(socketSet, *client.getSocket());
                        SDLNet_TCP_Close(*client.getSocket());
                        clients.erase(clients.begin() + i);
                        log::write << "Error receiving from client: " << SDLNet_GetError() << log::endl;
                        --i;  // Adjust index due to client removal
                    } else if (received == -1) {
                        // Error case
                        log::write << "Error receiving from client: " << SDLNet_GetError() << log::endl;
                    }
                }
            }
        }

        bool onConnect(std::string &endpoint) override
        {
            return true;
        }

        void onDisconnect() override
        {
            for(Client client : clients) {
                SDLNet_TCP_Close(*client.getSocket());
            }
            clients.clear();
            if(serverSocket) {
                SDLNet_TCP_Close(serverSocket);
            }
            delete[] buffer;
            log::write << "Server shut down" << log::endl;
        }

        void onShutdown() override
        {
            onDisconnect();
            SDLNet_Quit();
        }

    public:
        ServerNetHandler()
        {
            buffer = new char[1024];
        }
    };
}

#endif
#endif //SERVERNETHANDLER_H
