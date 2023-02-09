#include "../Window.h"
#include "../../../logging/Logger.h"
#include "../../strings/Strings.h"

#include <cassert>
#include <stdexcept>
#include <tuple>

#include <Windows.h>

#define internal static

namespace PGM::Platform
{

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
    case WM_DESTROY: {
        PostQuitMessage(0);
    }
    break;

    default:
        Result = DefWindowProcW(Window, Message, WParam, LParam);
        break;
    }

    return Result;
}

struct window_context_t
{
    WNDCLASSW wc;
    HWND window_handle;
    HDC hdc;
};

struct PGM::Platform::Window::window_impl_t
{
    window_context_t ctx;

    inline window_impl_t(window_context_t _ctx) : ctx{std::move(_ctx)}
    {
    }
};

internal window_context_t win32_create_window(const std::string_view &title, unsigned w, unsigned h,
                                              Window::FLAGS flags)
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);

    window_context_t context{};

    context.wc = {};
    context.wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    context.wc.lpfnWndProc = WindowProc;
    context.wc.hInstance = hInstance;
    context.wc.lpszClassName = L"RENDERERWND";
    if (!RegisterClassW(&context.wc))
    {
        DWORD Error = GetLastError();
        PGM::Logging::log_error("RegisterClass failed with err code: {}", Error);
    }
    else
    {
        context.window_handle =
            CreateWindowExW(0, context.wc.lpszClassName, Strings::widen_utf8(title).c_str(),
                            WS_OVERLAPPEDWINDOW, // | WS_VISIBLE,
                            CW_USEDEFAULT, CW_USEDEFAULT, (int)w, (int)h, NULL, NULL, hInstance, NULL);

        if (context.window_handle)
        {
            context.hdc = GetDC(context.window_handle);
        }
    }

    return context;
}

Window::Window(const std::string_view &title, unsigned w /*= 800*/, unsigned h /*= 600*/, FLAGS flags /*= bDefault*/)
{
    auto ctx = win32_create_window(title, w, h, flags);
    if (!ctx.window_handle)
    {
        throw std::runtime_error{"Cant create the window..."};
    }

    m_Impl = std::make_unique<window_impl_t>(std::move(ctx));
}

Window::~Window() = default;

void Window::run() const
{
    assert(m_Impl && m_Impl->ctx.window_handle);

    ShowWindow(m_Impl->ctx.window_handle, 1);

    for (;;)
    {
        MSG Message;
        BOOL MessageResult = GetMessage(&Message, m_Impl->ctx.window_handle, 0, 0);
        if (MessageResult > 0)
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        else
        {
            break;
        }
    }
}

} // namespace PGM::Platform