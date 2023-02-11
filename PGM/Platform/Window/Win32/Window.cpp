#include "../Window.h"
#include "../../Strings/Strings.h"
#include "../Events/WindowEvents.h"
#include "WindowImpl.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>
#include <cassert>
#include <stdexcept>
#include <tuple>

#include <Windows.h>

#define internal static

namespace PGM::Platform
{

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Logging::log_debug("(Win32) WindowProc: Message={}", message);

    LRESULT Result = 0;

    switch (message)
    {
    case WM_DESTROY: {
        PostQuitMessage(0);
    }
    break;

    case WM_SIZE: {
        Events::EventDispatcher *dispatcher =
            reinterpret_cast<Events::EventDispatcher *>(GetWindowLongPtr(window, GWLP_USERDATA));
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        // Resized
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        dispatcher->emplace_dispatch<WindowEvents::WindowResizedEvent>(static_cast<int>(width),
                                                                       static_cast<int>(height));
    }
    default:
        Result = DefWindowProcW(window, message, wParam, lParam);
        break;
    }

    return Result;
}

internal Window::window_impl_t win32_create_window(const std::string_view &title, unsigned w, unsigned h,
                                                   Window::FLAGS flags)
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);

    Window::window_impl_t context{};

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

    Logging::log_info("{}", "(Win32) Window handle created");
    SetWindowLongPtr(ctx.window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_Dispatcher));

    m_Impl = std::make_unique<window_impl_t>(std::move(ctx));
}

Window::~Window() = default;

const Window::window_impl_t &Window::impl() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");
    return *m_Impl;
}

RectInt Window::rect() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    RECT rect;
#ifdef PGM_ASSERTS_ENABLED
    PGM_ASSERT(GetWindowRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetWindowRect(m_Impl->window_handle, &rect)
#endif

    return RectInt{static_cast<int>(rect.left), static_cast<int>(rect.bottom), static_cast<int>(rect.right - rect.left),
                   static_cast<int>(rect.bottom - rect.top)};
}

int Window::width() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    RECT rect;
#ifdef PGM_ASSERTS_ENABLED
    PGM_ASSERT(GetWindowRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetWindowRect(m_Impl->window_handle, &rect)
#endif

    return static_cast<int>(rect.right - rect.left);
}

int Window::height() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    RECT rect;
#ifdef PGM_ASSERTS_ENABLED
    PGM_ASSERT(GetWindowRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetWindowRect(m_Impl->window_handle, &rect)
#endif

    return static_cast<int>(rect.bottom - rect.top);
}

void Window::show() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");
    ShowWindow(m_Impl->window_handle, 1);
}

bool Window::pumpMessages() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    MSG Message;
    BOOL MessageResult = GetMessage(&Message, m_Impl->window_handle, 0, 0);
    if (MessageResult > 0)
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    else
    {
        return false;
    }

    return true;
}

} // namespace PGM::Platform