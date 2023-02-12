#include "../Mouse.h"

#include <PGM/Core/Assert/Assert.h>

#include <Windows.h>

namespace PGM::Platform::Input
{

bool isMouseButtonDown(MouseButton button)
{
    switch (button)
    {
    case MouseLeft:
        return GetAsyncKeyState(VK_LBUTTON) != 0;
    case MouseMiddle:
        return GetAsyncKeyState(VK_MBUTTON) != 0;
    case MouseRight:
        return GetAsyncKeyState(VK_RBUTTON) != 0;
    default:
        PGM_ASSERT(false, "Invalid mouse button");
        return false;
    }
}

} // namespace PGM::Platform::Input