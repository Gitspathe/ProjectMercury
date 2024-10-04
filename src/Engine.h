#ifndef ENGINE_H
#define ENGINE_H

#include "Engine/log.h"
using namespace GameLogging;

namespace Engine {}
namespace Engine::Common {}

namespace Engine::Core::SubsystemExecOrder
{
    constexpr uint32_t SERVER_CMD_MANAGER = 1;
    constexpr uint32_t IMGUI              = 10;
    constexpr uint32_t INPUT_MANAGER      = 100;
    constexpr uint32_t NET_MANAGER        = 200;
}

using namespace Engine;
using namespace Engine::Common;

#endif //ENGINE_H
