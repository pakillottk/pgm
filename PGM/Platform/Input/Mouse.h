#pragma once

#include "../Window/Window.h"

#include "../../Core/Vec/Vec.h"

namespace PGM::Input
{

enum MouseButton
{
    MouseLeft,
    MouseMiddle,
    MouseRight
};

bool isMouseButtonDown(MouseButton button);
Vec2 mousePosition(const Window &wnd);

} // namespace PGM::Input