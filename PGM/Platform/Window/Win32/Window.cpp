#include "../Window.h"
#include "../../Strings/Strings.h"
#include "../Events/WindowEvents.h"

#include "WindowImpl.h"

#include "../../../Core/Assert/Assert.h"
#include "../../../Core/Logging/Logger.h"

#include <cassert>
#include <stdexcept>
#include <tuple>

#include <Windows.h>
#include <Windowsx.h>

#define internal static

namespace PGM
{

static constexpr Input::Key convertFromPlatformKey(int nativeKey)
{
    switch (nativeKey)
    {
    case 'A':
        return Input::A;
    case 'B':
        return Input::B;
    case 'C':
        return Input::C;
    case 'D':
        return Input::D;
    case 'E':
        return Input::E;
    case 'F':
        return Input::F;
    case 'G':
        return Input::G;
    case 'H':
        return Input::H;
    case 'I':
        return Input::I;
    case 'J':
        return Input::J;
    case 'K':
        return Input::K;
    case 'L':
        return Input::L;
    case 'M':
        return Input::M;
    case 'N':
        return Input::N;
    case 'O':
        return Input::O;
    case 'P':
        return Input::P;
    case 'Q':
        return Input::Q;
    case 'R':
        return Input::R;
    case 'S':
        return Input::S;
    case 'T':
        return Input::T;
    case 'U':
        return Input::U;
    case 'V':
        return Input::V;
    case 'W':
        return Input::W;
    case 'X':
        return Input::X;
    case 'Y':
        return Input::Y;
    case 'Z':
        return Input::Z;
    case '0':
        return Input::Num0;
    case '1':
        return Input::Num1;
    case '2':
        return Input::Num2;
    case '3':
        return Input::Num3;
    case '4':
        return Input::Num4;
    case '5':
        return Input::Num5;
    case '6':
        return Input::Num6;
    case '7':
        return Input::Num7;
    case '8':
        return Input::Num8;
    case '9':
        return Input::Num9;
    case VK_ESCAPE:
        return Input::Escape;
    case VK_LCONTROL:
        return Input::LControl;
    case VK_LSHIFT:
        return Input::LShift;
    case VK_LMENU:
        return Input::LAlt;
    case VK_LWIN:
        return Input::LSystem;
    case VK_RCONTROL:
        return Input::RControl;
    case VK_RSHIFT:
        return Input::RShift;
    case VK_RMENU:
        return Input::RAlt;
    case VK_RWIN:
        return Input::RSystem;
    case VK_APPS:
        return Input::Menu;
    case VK_OEM_4:
        return Input::LBracket;
    case VK_OEM_6:
        return Input::RBracket;
    case VK_OEM_1:
        return Input::Semicolon;
    case VK_OEM_COMMA:
        return Input::Comma;
    case VK_OEM_PERIOD:
        return Input::Period;
    case VK_OEM_7:
        return Input::Apostrophe;
    case VK_OEM_2:
        return Input::Slash;
    case VK_OEM_5:
        return Input::Backslash;
    case VK_OEM_3:
        return Input::Grave;
    case VK_OEM_PLUS:
        return Input::Equal;
    case VK_OEM_MINUS:
        return Input::Hyphen;
    case VK_SPACE:
        return Input::Space;
    case VK_RETURN:
        return Input::Enter;
    case VK_BACK:
        return Input::Backspace;
    case VK_TAB:
        return Input::Tab;
    case VK_PRIOR:
        return Input::PageUp;
    case VK_NEXT:
        return Input::PageDown;
    case VK_END:
        return Input::End;
    case VK_HOME:
        return Input::Home;
    case VK_INSERT:
        return Input::Insert;
    case VK_DELETE:
        return Input::Delete;
    case VK_ADD:
        return Input::Add;
    case VK_SUBTRACT:
        return Input::Subtract;
    case VK_MULTIPLY:
        return Input::Multiply;
    case VK_DIVIDE:
        return Input::Divide;
    case VK_LEFT:
        return Input::Left;
    case VK_RIGHT:
        return Input::Right;
    case VK_UP:
        return Input::Up;
    case VK_DOWN:
        return Input::Down;
    case VK_NUMPAD0:
        return Input::Numpad0;
    case VK_NUMPAD1:
        return Input::Numpad1;
    case VK_NUMPAD2:
        return Input::Numpad2;
    case VK_NUMPAD3:
        return Input::Numpad3;
    case VK_NUMPAD4:
        return Input::Numpad4;
    case VK_NUMPAD5:
        return Input::Numpad5;
    case VK_NUMPAD6:
        return Input::Numpad6;
    case VK_NUMPAD7:
        return Input::Numpad7;
    case VK_NUMPAD8:
        return Input::Numpad8;
    case VK_NUMPAD9:
        return Input::Numpad9;
    case VK_F1:
        return Input::F1;
    case VK_F2:
        return Input::F2;
    case VK_F3:
        return Input::F3;
    case VK_F4:
        return Input::F4;
    case VK_F5:
        return Input::F5;
    case VK_F6:
        return Input::F6;
    case VK_F7:
        return Input::F7;
    case VK_F8:
        return Input::F8;
    case VK_F9:
        return Input::F9;
    case VK_F10:
        return Input::F10;
    case VK_F11:
        return Input::F11;
    case VK_F12:
        return Input::F12;
    case VK_F13:
        return Input::F13;
    case VK_F14:
        return Input::F14;
    case VK_F15:
        return Input::F15;
    case VK_PAUSE:
        return Input::Pause;
    default:
        return Input::Unknown;
    }
}

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

    return convertFromPlatformKey(static_cast<int>(key));
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
        dispatcher->emplace_dispatch<WindowEvents::WindowResized>(static_cast<int>(width), static_cast<int>(height));
        Result = DefWindowProcW(window, message, wParam, lParam);
    }
    break;

    case WM_MOUSEMOVE: {
        const auto x = GET_X_LPARAM(lParam);
        const auto y = GET_Y_LPARAM(lParam);

        dispatcher->emplace_dispatch<WindowEvents::MouseMove>(Vec2{static_cast<float>(x), static_cast<float>(y)});
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
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonUp>(Input::MouseLeft);
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
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonUp>(Input::MouseMiddle);
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
        dispatcher->emplace_dispatch<WindowEvents::MouseButtonUp>(Input::MouseRight);
    }
    break;

    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        const auto key = vkeyToPgmKey(wParam, lParam);
        bool repeat = (HIWORD(lParam) & KF_REPEAT) != 0;
        dispatcher->emplace_dispatch<WindowEvents::WindowKeyDown>(key, repeat);
    }
    break;

    case WM_SYSKEYUP:
    case WM_KEYUP: {
        PGM_ASSERT(dispatcher != nullptr, "Window EventDispatcher not set");

        const auto key = vkeyToPgmKey(wParam, lParam);
        dispatcher->emplace_dispatch<WindowEvents::WindowKeyUp>(key);
    }
    break;

    case WM_CHAR: {
#ifndef UNICODE
        PGM_ASSERT(false, "Win32 should use Unicode API!");
#endif
        if (wParam > 0 && wParam < 0x10000)
        {
            wchar_t chars[2] = {(wchar_t)wParam, NULL};
            int utf8len = WideCharToMultiByte(CP_UTF8, 0, chars, 1, NULL, 0, NULL, NULL);

            std::string utf8;
            utf8.resize(utf8len);

            ::WideCharToMultiByte(CP_UTF8, 0, chars, 1, &utf8[0], utf8len, 0, 0);

            dispatcher->emplace_dispatch<WindowEvents::WindowTextInput>(std::move(utf8));
        }

        Result = DefWindowProcW(window, message, wParam, lParam);
    }
    break;

    case WM_CLOSE: {
        dispatcher->emplace_dispatch<WindowEvents::WindowClose>();
        Result = DefWindowProcW(window, message, wParam, lParam);
        PostQuitMessage(0);
    }
    break;

    default:
        Result = DefWindowProcW(window, message, wParam, lParam);
        break;
    }

    return Result;
}

internal Window::window_impl_t win32_create_window(std::string_view title, unsigned w, unsigned h,
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

Window::Window(std::string_view title, unsigned w /*= 800*/, unsigned h /*= 600*/, FLAGS flags /*= bDefault*/)
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
    PGM_ASSERT(GetClientRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetClientRect(m_Impl->window_handle, &rect);
#endif

    return RectInt{static_cast<int>(rect.left), static_cast<int>(rect.bottom), static_cast<int>(rect.right - rect.left),
                   static_cast<int>(rect.bottom - rect.top)};
}

int Window::width() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    RECT rect;
#ifdef PGM_ASSERTS_ENABLED
    PGM_ASSERT(GetClientRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetClientRect(m_Impl->window_handle, &rect);
#endif

    return static_cast<int>(rect.right - rect.left);
}

int Window::height() const
{
    PGM_ASSERT(m_Impl && m_Impl->window_handle, "Invalid window context");

    RECT rect;
#ifdef PGM_ASSERTS_ENABLED
    PGM_ASSERT(GetClientRect(m_Impl->window_handle, &rect) == TRUE, "Windows API call failed");
#else
    GetClientRect(m_Impl->window_handle, &rect);
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

    MSG message;
    BOOL hasMessage = PeekMessage(&message, m_Impl->window_handle, 0, 0, PM_NOREMOVE);
    if (hasMessage == FALSE)
    {
        // does not means that app is over...
        return true;
    }

    BOOL messageResult = GetMessage(&message, m_Impl->window_handle, 0, 0);
    if (messageResult == TRUE)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
        return true;
    }

    return false;
}

} // namespace PGM