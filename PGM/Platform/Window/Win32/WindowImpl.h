#pragma once

#include "../Window.h"
#include <Windows.h>

namespace PGM
{

struct Window::window_impl_t
{
    WNDCLASSW wc;
    HWND window_handle;
    HDC hdc;
};

} // namespace PGM