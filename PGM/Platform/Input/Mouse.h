#pragma once

namespace PGM::Platform::Input
{

enum MouseButton
{
    MouseLeft,
    MouseMiddle,
    MouseRight
};

bool isMouseButtonDown(MouseButton button);

} // namespace PGM::Platform::Input