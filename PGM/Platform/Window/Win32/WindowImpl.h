#pragma once

#include "../Window.h"
#include <Windows.h>

namespace PGM::Platform
{

struct Window::window_impl_t
{
    WNDCLASSW wc;
    HWND window_handle;
    HDC hdc;
};

} // namespace PGM::Platform