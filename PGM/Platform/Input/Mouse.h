#pragma once

#include "../Window/Window.h"

#include <PGM/Core/Vec/Vec.h>

namespace PGM::Platform::Input
{

enum MouseButton
{
    MouseLeft,
    MouseMiddle,
    MouseRight
};

bool isMouseButtonDown(MouseButton button);
Vec2 mousePosition(const Window &wnd);

} // namespace PGM::Platform::Input