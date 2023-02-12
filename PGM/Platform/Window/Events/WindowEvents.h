#pragma once

#include "../../Input/Keyboard.h"

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

} // namespace PGM::Platform::WindowEvents
