#include "../Mouse.h"
#include "../../Window/Win32/WindowImpl.h"

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

Vec2 mousePosition(const Window &wnd)
{
    POINT pos;
    if (::GetCursorPos(&pos) && ::ScreenToClient(wnd.impl().window_handle, &pos))
    {
        return Vec2{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    }

    return Vec2{};
}

} // namespace PGM::Platform::Input