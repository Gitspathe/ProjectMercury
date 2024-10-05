#ifndef SERVERNETHANDLER_H
#define SERVERNETHANDLER_H
#if SERVER && !CLIENT
#include "TestPacketHandler.h"
#include "NetHandler.h"
#include "SDL2/SDL_net.h"
#include "Peer.h"
#include "../../Engine.h"

namespace Engine::Net
{
    class ServerNetHandler final : public NetHandler
    {
    protected:
        IPaddress ipAddr{};
        TCPsocket serverSocket = nullptr;
        std::vector<std::shared_ptr<Peer>> clients = std::vector<std::shared_ptr<Peer>>();
        std::unordered_map<TCPsocket, std::shared_ptr<Peer>> clientSockets;
        uint8_t* buffer = nullptr;
        uint8_t* packetBuffer = nullptr;
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

        void handleServerSocket()
        {
            TCPsocket newClientSocket = SDLNet_TCP_Accept(serverSocket);
            if(newClientSocket == nullptr)
                return;

            PeerUID id;
            if(!Peer::generateUID(id, netManager->getPeers(), 256)) {
                log::write << "Failed to generate a random ID for the client." << log::endl;
                SDLNet_TCP_Close(newClientSocket);
                return;
            }

            auto newPeer = std::make_shared<Peer>(newClientSocket, id);
            if(!netManager->tryRegisterPeer(newPeer)) {
                log::write << "Failed to register peer for the client." << log::endl;
                SDLNet_TCP_Close(newClientSocket);
                return;
            }

            const IPaddress* addr = SDLNet_TCP_GetPeerAddress(newClientSocket);
            log::write << "New client connected! host: " << addr->host << " port:" << addr->port << log::endl;

            int addSocketOp = SDLNet_TCP_AddSocket(socketSet, newClientSocket);
            if(addSocketOp == -1) {
                log::write << "Failed to add client socket to set: " << SDLNet_GetError() << log::endl;
                SDLNet_TCP_Close(newClientSocket);
                return;
            }

            if(SDLNet_TCP_AddSocket(socketSet, newClientSocket) == -1) {
                log::write << "Failed to add client socket to set: " << SDLNet_GetError() << log::endl;
                SDLNet_TCP_Close(newClientSocket);
            } else {
                clientSockets[newClientSocket] = newPeer;
                clients.push_back(newPeer);
            }
        }

        bool handlePeerSocket(const std::shared_ptr<Peer>& peer)
        {
            if(!SDLNet_SocketReady(peer->getSocket()))
                return true;

            int received = SDLNet_TCP_Recv(peer->getSocket(), buffer, MAX_PACKET_SIZE);
            if(received > 0) {
                netManager->getPacketManager().onMessage(*peer, buffer, received);

                for(int i = 0; i < 256; i++) {
                    std::string msg = "Hello from my new packet handler thing!";
                    PacketHandler* p = netManager->getPacketManager().getHandler<TestPacketHandler>(PacketTypes::TEST_MSG);
                    Packet packet = p->construct(&msg);
                    send(*peer, packet);
                }
                return true;
            }
            if(received < 0) {
                log::write << "Error receiving from client: " << SDLNet_GetError() << log::endl;
                return true;
            }

            // 0 = disconnection.
            netManager->tryUnregisterPeer(peer);
            log::write << "Client disconnected" << log::endl;
            SDLNet_TCP_DelSocket(socketSet, peer->getSocket());
            SDLNet_TCP_Close(peer->getSocket());
            return false;
        }

        void onUpdate(const float deltaTime) override
        {
            if(SDLNet_CheckSockets(socketSet, 0) <= 0)
                return;

            if(SDLNet_SocketReady(serverSocket)) {
                handleServerSocket();
            }
            for(size_t i = 0; i < clients.size(); ++i) {
                if(!handlePeerSocket(clients[i])) {
                    clients.erase(clients.begin() + i);
                    i--;
                }
            }
        }

        bool onConnect(std::string &endpoint) override
        {
            return true;
        }

        void onDisconnect() override
        {
            for(const std::shared_ptr<Peer>& peer : clients) {
                SDLNet_TCP_Close(peer->getSocket());
            }
            clients.clear();
            clientSockets.clear();
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
        void send(Peer& peer, Packet& packet) override
        {
            uint16_t seq = 0;
            if(!netManager->tryIncrementSeq(peer.getUID(), seq)) {
                log::write << "Failed to get peer seq from client " << peer.getUID() << log::endl;
                return;
            }
            packet.setSeq(seq);
            packet.writeInto(packetBuffer);
            SDLNet_TCP_Send(peer.getSocket(), packetBuffer, packet.getFullSize());
        }

        ServerNetHandler()
        {
            packetBuffer = new uint8_t[MAX_PACKET_SIZE];
            buffer = new uint8_t[MAX_PACKET_SIZE];
        }
    };
}

#endif
#endif //SERVERNETHANDLER_H
