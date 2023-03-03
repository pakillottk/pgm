#pragma once

#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"

namespace PGM::WindowEvents
{

struct WindowResized final
{
    int width;
    int height;
};

struct WindowKeyDown final
{
    Input::Key key;
    bool repeat;
};

struct WindowKeyUp final
{
    Input::Key key;
};

struct WindowTextInput final
{
    std::string input;
};

struct MouseMove final
{
    Vec2 location;
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

struct WindowClose final
{
};

} // namespace PGM::Platform::WindowEvents
