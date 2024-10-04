#ifndef SERVERCOMMANDMANAGER_H
#define SERVERCOMMANDMANAGER_H
#if SERVER && !CLIENT
#include "Subsystem.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "../../Engine.h"

namespace Engine::Core
{
    class ServerCommandManager final : public Subsystem
    {
    protected:
        std::thread cmdThread;
        std::atomic<bool> running = true;

        // Thread-safe queue for commands
        std::queue<std::string> commandQueue;
        std::mutex queueMutex;
        std::condition_variable queueCondVar;

        bool onInit() override
        {
            cmdThread = std::thread(&ServerCommandManager::inputHandler, this);
            return true;
        }

        void onUpdate(float deltaTime) override
        {
            processCommands();
        }

        void processCommands()
        {
            std::unique_lock lock(queueMutex);
            while(!commandQueue.empty()) {
                std::string command = commandQueue.front();
                commandQueue.pop();
                lock.unlock();

                if(command == "exit") {
                    running = false;
                    exit();
                    break;
                }

                log::write(log::serverCmd) << "Processing command: " << command << log::endl;
            }
        }

        void exit() const
        {
            log::write(log::serverCmd) << "Exiting." << log::endl;
            if (const std::shared_ptr<Game> runningGame = game.lock(); runningGame != nullptr) {
                runningGame->quit();
            }
        }

        void inputHandler()
        {
            while(running) {
                std::string input;
                std::cout << "> ";
                std::getline(std::cin, input);
                if(input.empty())
                    continue;

                {
                    std::lock_guard lock(queueMutex);
                    commandQueue.push(input);
                }

                queueCondVar.notify_one();
            }
        }

    public:
        ~ServerCommandManager() override
        {
            running = false;
            queueCondVar.notify_all();
            if(cmdThread.joinable()) {
                cmdThread.join();
            }
        }

        std::string getName() override
        {
            return "ServerCommandManager";
        }

        uint32_t getOrder() override
        {
            return SubsystemExecOrder::SERVER_CMD_MANAGER;
        }
    };
}

#endif
#endif //SERVERCOMMANDMANAGER_H
