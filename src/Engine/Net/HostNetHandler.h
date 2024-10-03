#ifndef HOSTNETHANDLER_H
#define HOSTNETHANDLER_H
#if CLIENT && SERVER
#include "NetHandler.h"

namespace Engine::Net
{
    class HostNetHandler : public NetHandler
    {

    };
}

#endif
#endif //HOSTNETHANDLER_H
