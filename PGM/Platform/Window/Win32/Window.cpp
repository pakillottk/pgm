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

static inline Input::Key vkeyToPgmKey(WPARAM wParam, LPARAM lParam)
{
    auto key = wParam;
    switch (key)
    {
    // Check the scancode to distinguish between left and right shift
    case VK_SHIFT: {
        static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
        UINT scancode = static_cast<UINT>((lParam & (0xFF << 16)) >> 16);
        key = scancode == lShift ? Input::LShift : Input::RShift;
    }
    break;

    // Check the "extended" flag to distinguish between left and right alt
    case VK_MENU:
        key = (HIWORD(lParam) & KF_EXTENDED) ? Input::RAlt : Input::LAlt;
        break;
    // Check the "extended" flag to distinguish between left and right control
    case VK_CONTROL:
        key = (HIWORD(lParam) & KF_EXTENDED) ? Input::RControl : Input::LControl;
        break;
    }

    return Input::detail::convertFromPlatformKey(static_cast<int>(key));
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    Events::EventDispatcher *dispatcher =
        reinterpret_cast<Events::EventDispatcher *>(GetWindowLongPtr(window, GWLP_USERDATA));
    // Logging::log_debug("(Win32) WindowProc: Message={}", message);

    LRESULT Result = 0;
    switch (message)
    {
    case WM_DESTROY: {
        PostQuitMessage(0);
    }
    break;

    case WM_SIZE: {
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        // Resized
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        dispatcher->emplace_dispatch<WindowEvents::WindowResizedEvent>(static_cast<int>(width),
                                                                       static_cast<int>(height));
        Result = DefWindowProcW(window, message, wParam, lParam);
    }
    break;

    case WM_LBUTTONDOWN: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseLeft, false);
    }
    break;
    case WM_LBUTTONDBLCLK: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseLeft, true);
    }
    break;
    case WM_LBUTTONUP: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseLeft);
    }
    break;

    case WM_MBUTTONDOWN: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseMiddle, false);
    }
    break;
    case WM_MBUTTONDBLCLK: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseMiddle, true);
    }
    break;
    case WM_MBUTTONUP: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseMiddle);
    }
    break;

    case WM_RBUTTONDOWN: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseRight, false);
    }
    break;
    case WM_RBUTTONDBLCLK: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseRight, true);
    }
    break;
    case WM_RBUTTONUP: {
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonDown>(Input::MouseRight);
    }
    break;

    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        const auto key = vkeyToPgmKey(wParam, lParam);
        dispatcher->emplace_dispatch<WindowEvents::WindowKeyDown>(key);
    }
    break;

    case WM_SYSKEYUP:
    case WM_KEYUP: {
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        const auto key = vkeyToPgmKey(wParam, lParam);
        dispatcher->emplace_dispatch<WindowEvents::WindowKeyUp>(key);
    }
    break;

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
    : m_FullScreen{false}
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
    GetWindowRect(m_Impl->window_handle, &rect);
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
    GetWindowRect(m_Impl->window_handle, &rect);
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
    GetWindowRect(m_Impl->window_handle, &rect);
#endif

    return static_cast<int>(rect.bottom - rect.top);
}

void Window::show()
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");
    ShowWindow(m_Impl->window_handle, 1);
}

void Window::setFullScreen(bool v)
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    if (m_FullScreen == v)
    {
        return;
    }

    if (v)
    {
        Logging::log_debug("(Win32) {}", "Entering fullscreen");

        DEVMODE fullscreenSettings;

        const auto screenWidth = GetDeviceCaps(m_Impl->hdc, HORZRES);
        const auto screenHeight = GetDeviceCaps(m_Impl->hdc, VERTRES);

        int colourBits = GetDeviceCaps(m_Impl->hdc, BITSPIXEL);
        int refreshRate = GetDeviceCaps(m_Impl->hdc, VREFRESH);

        if (EnumDisplaySettings(NULL, 0, &fullscreenSettings) == FALSE)
        {
            Logging::log_error("(Win32) {}", "EnumDisplaySettings failed");
        }

        fullscreenSettings.dmPelsWidth = screenWidth;
        fullscreenSettings.dmPelsHeight = screenHeight;
        fullscreenSettings.dmBitsPerPel = colourBits;
        fullscreenSettings.dmDisplayFrequency = refreshRate;
        fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

        SetWindowLongPtr(m_Impl->window_handle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
        SetWindowLongPtr(m_Impl->window_handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(m_Impl->window_handle, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
        if (ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            Logging::log_error("(Win32) {}", "ChangeDisplaySettings failed");
        }
        ShowWindow(m_Impl->window_handle, SW_MAXIMIZE);
        Logging::log_info("(Win32) {}", "Entered full screen mode");
    }
    else
    {
        Logging::log_debug("(Win32) {}", "Exit fullscreen");

        SetWindowLongPtr(m_Impl->window_handle, GWL_EXSTYLE, WS_EX_LEFT);
        SetWindowLongPtr(m_Impl->window_handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        if (ChangeDisplaySettings(NULL, CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
        {
            Logging::log_error("(Win32) {}", "ChangeDisplaySettings failed");
        }
        SetWindowPos(m_Impl->window_handle, HWND_NOTOPMOST, 0, 0, 800, 600, SWP_SHOWWINDOW);
        ShowWindow(m_Impl->window_handle, SW_RESTORE);

        Logging::log_info("(Win32) {}", "Exited full screen mode");
    }

    m_FullScreen = v;
}

bool Window::pumpMessages()
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    // TODO(pgm) Must detect the window destruction to return false
    MSG message;
    BOOL messageResult = PeekMessage(&message, m_Impl->window_handle, 0, 0, PM_REMOVE);
    if (messageResult == TRUE)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
        return message.message != WM_QUIT && message.message != WM_CLOSE;
    }

    return true;
}

} // namespace PGM::Platform