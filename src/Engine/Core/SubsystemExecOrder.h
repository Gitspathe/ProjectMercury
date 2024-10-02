#ifndef SUBSYSTEMORDER_H
#define SUBSYSTEMORDER_H
#include <cstdint>

namespace Engine::Core
{
    namespace SubsystemExecOrder
    {
        constexpr uint32_t SERVER_CMD_MANAGER = 1;
        constexpr uint32_t IMGUI              = 10;
        constexpr uint32_t INPUT_MANAGER      = 100;
    }
}

#endif //SUBSYSTEMORDER_H
