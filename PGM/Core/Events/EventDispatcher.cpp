#include "EventDispatcher.h"

namespace PGM::Events
{

std::atomic<listener_id> EventQueue::g_LastListenerId = 0;

}