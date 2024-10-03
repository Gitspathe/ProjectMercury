#ifndef NETMANAGER_H
#define NETMANAGER_H
#include <memory>
#include <string>
#include "HostNetHandler.h"
#include "ClientNetHandler.h"
#include "ServerNetHandler.h"
#include "../../Engine.h"
#include "../Core/Subsystem.h"
#include "../Core/SubsystemExecOrder.h"

namespace Engine::Net
{
    class NetManager final : public Core::Subsystem
    {
    public:

        static uint32_t generateUID();

        std::string getName() override
        {
            return "NetManager";
        }

        uint32_t getOrder() override
        {
            return Core::SubsystemExecOrder::NET_MANAGER;
        }

        bool connect(const std::string& endpoint) const
        {
            if(handler == nullptr) {
                log::write << "Connection failed - NetHandler is NULL." << log::endl;
                return false;
            }
            if(!handler->connect(endpoint)) {
                log::write << "Connection failed." << log::endl;
                return false;
            }
            return true;
        }

        void disconnect() const
        {
            if(handler == nullptr)
                return;

            handler->shutdown();
        }

    protected:
        std::unique_ptr<NetHandler> handler;

        bool onInit() override
        {
#if CLIENT && !SERVER
            log::write << "Initializing NetHandler as a client." << log::endl;
            handler = std::make_unique<ClientNetHandler>();
#elif SERVER && !CLIENT
            log::write << "Initializing NetHandler as the server." << log::endl;
            handler = std::make_unique<ServerNetHandler>();
#else
            log::write << "Initializing NetHandler as the host." << log::endl;
            handler = std::make_unique<HostNetHandler>();
#endif

            if(handler == nullptr || !handler->init()) {
                log::write << "Failed to initialize NetManager" << log::endl;
                return false;
            }
            return true;
        }

        void onDestroy() override
        {
            disconnect();
        }

        void onUpdate(float deltaTime) override
        {
            if(handler != nullptr) {
                handler->update(deltaTime);
            }
        }
    };

    static uint32_t generateUID()
    {
        return rand() % 10000;
    }
}

#endif //NETMANAGER_H
