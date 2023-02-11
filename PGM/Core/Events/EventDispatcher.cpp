#include "EventDispatcher.h"

namespace PGM::Events
{

std::atomic<listener_id> EventDispatcher::g_LastListenerId = 0;

}