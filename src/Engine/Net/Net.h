#ifndef NET_H
#define NET_H
#include <cstdint>
#include <functional>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace Engine::Net::PacketTypes
{
    constexpr uint8_t INVALID = 0;
    constexpr uint8_t TEST_MSG = 1;
    constexpr uint8_t RPC = 2;
}

namespace Engine::Net
{
    typedef uint64_t PeerUID;
    static constexpr uint16_t MAX_PACKET_SIZE = 1024;
    static constexpr uint16_t PACKET_HEADER_SIZE = 5;

    class RPCParams {
    public:
        template<typename... Args>
        RPCParams(Args&&... args) : params(std::forward<Args>(args)...) {}

        template<size_t Index>
        auto get() const {
            return std::get<Index>(params);
        }

    private:
        std::tuple<> params;  // Empty tuple as a base
    };

    inline std::unordered_map<std::string, std::function<void(void*, RPCParams&)>> rpc_registry;

    #define DEF_RPC(ClassName, FuncName, ...) \
    void ClassName::FuncName(RPCParams& params)

    #define REGISTER_RPC(ClassName, FuncName) \
    rpc_registry[#FuncName] = [](void* obj, RPCParams& params) { \
        auto instance = static_cast<ClassName*>(obj); \
        instance->FuncName(params); \
    }

    #define CALL_RPC(ClassObj, FuncName, ...) \
    if (rpc_registry.find(#FuncName) != rpc_registry.end()) { \
        RPCParams params(__VA_ARGS__); \
        rpc_registry[#FuncName](&ClassObj, params); \
    } else { \
        std::cerr << "RPC " << #FuncName << " not found!" << std::endl; \
    }
}

#endif //NET_H
