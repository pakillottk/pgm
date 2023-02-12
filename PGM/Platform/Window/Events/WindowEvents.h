#pragma once

#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"

namespace PGM::Platform::WindowEvents
{

struct WindowResizedEvent final
{
    int width;
    int height;
};

struct WindowKeyDown final
{
    Input::Key key;
};

struct WindowKeyUp final
{
    Input::Key key;
};

struct MouseButtonDown final
{
    Input::MouseButton button;
    bool isDoubleClick;
};

struct MouseButtonUp final
{
    Input::MouseButton button;
};

} // namespace PGM::Platform::WindowEvents
