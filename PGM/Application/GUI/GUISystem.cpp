#include "GUISystem.h"
#include "../Application.h"

#include <PGM/Platform/Input/Keyboard.h>
#include <PGM/Platform/Input/Mouse.h>

// Compile IMGUI
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_tables.cpp>
#include <imgui/imgui_widgets.cpp>

// IMGUI Demo
#include <imgui/imgui_demo.cpp>

#include <imgui/imgui.h>

namespace PGM::GUI
{

static constexpr ImGuiKey convertToImGuiKey(Platform::Input::Key key)
{
    switch (key)
    {
    case Platform::Input::Key::A:
        return ImGuiKey_A;
    case Platform::Input::Key::B:
        return ImGuiKey_B;
    case Platform::Input::Key::C:
        return ImGuiKey_C;
    case Platform::Input::Key::D:
        return ImGuiKey_D;
    case Platform::Input::Key::E:
        return ImGuiKey_E;
    case Platform::Input::Key::F:
        return ImGuiKey_F;
    case Platform::Input::Key::G:
        return ImGuiKey_G;
    case Platform::Input::Key::H:
        return ImGuiKey_H;
    case Platform::Input::Key::I:
        return ImGuiKey_I;
    case Platform::Input::Key::J:
        return ImGuiKey_J;
    case Platform::Input::Key::K:
        return ImGuiKey_K;
    case Platform::Input::Key::L:
        return ImGuiKey_L;
    case Platform::Input::Key::M:
        return ImGuiKey_M;
    case Platform::Input::Key::N:
        return ImGuiKey_N;
    case Platform::Input::Key::O:
        return ImGuiKey_O;
    case Platform::Input::Key::P:
        return ImGuiKey_P;
    case Platform::Input::Key::Q:
        return ImGuiKey_Q;
    case Platform::Input::Key::R:
        return ImGuiKey_R;
    case Platform::Input::Key::S:
        return ImGuiKey_S;
    case Platform::Input::Key::T:
        return ImGuiKey_T;
    case Platform::Input::Key::U:
        return ImGuiKey_U;
    case Platform::Input::Key::V:
        return ImGuiKey_V;
    case Platform::Input::Key::W:
        return ImGuiKey_W;
    case Platform::Input::Key::X:
        return ImGuiKey_X;
    case Platform::Input::Key::Y:
        return ImGuiKey_Y;
    case Platform::Input::Key::Z:
        return ImGuiKey_Z;

    case Platform::Input::Key::Num0:
        return ImGuiKey_0;
    case Platform::Input::Key::Num1:
        return ImGuiKey_1;
    case Platform::Input::Key::Num2:
        return ImGuiKey_2;
    case Platform::Input::Key::Num3:
        return ImGuiKey_3;
    case Platform::Input::Key::Num4:
        return ImGuiKey_4;
    case Platform::Input::Key::Num5:
        return ImGuiKey_5;
    case Platform::Input::Key::Num6:
        return ImGuiKey_6;
    case Platform::Input::Key::Num7:
        return ImGuiKey_7;
    case Platform::Input::Key::Num8:
        return ImGuiKey_8;
    case Platform::Input::Key::Num9:
        return ImGuiKey_9;
    case Platform::Input::Key::Escape:
        return ImGuiKey_Escape;
    case Platform::Input::Key::LControl:
        return ImGuiKey_LeftCtrl;
    case Platform::Input::Key::LShift:
        return ImGuiKey_LeftShift;
    case Platform::Input::Key::LAlt:
        return ImGuiKey_LeftAlt;
    case Platform::Input::Key::LSystem:
        return ImGuiKey_LeftSuper;
    case Platform::Input::Key::RControl:
        return ImGuiKey_RightCtrl;
    case Platform::Input::Key::RShift:
        return ImGuiKey_RightShift;
    case Platform::Input::Key::RAlt:
        return ImGuiKey_RightAlt;
    case Platform::Input::Key::RSystem:
        return ImGuiKey_RightSuper;
    case Platform::Input::Key::Menu:
        return ImGuiKey_Menu;
    case Platform::Input::Key::LBracket:
        return ImGuiKey_LeftBracket;
    case Platform::Input::Key::RBracket:
        return ImGuiKey_RightBracket;
    case Platform::Input::Key::Semicolon:
        return ImGuiKey_Semicolon;
    case Platform::Input::Key::Comma:
        return ImGuiKey_Comma;
    case Platform::Input::Key::Period:
        return ImGuiKey_Period;
    case Platform::Input::Key::Apostrophe:
        return ImGuiKey_Apostrophe;
    case Platform::Input::Key::Slash:
        return ImGuiKey_Slash;
    case Platform::Input::Key::Backslash:
        return ImGuiKey_Backslash;
    case Platform::Input::Key::Grave:
        return ImGuiKey_GraveAccent;
    case Platform::Input::Key::Equal:
        return ImGuiKey_Equal;
    case Platform::Input::Key::Space:
        return ImGuiKey_Space;
    case Platform::Input::Key::Enter:
        return ImGuiKey_Enter;
    case Platform::Input::Key::Backspace:
        return ImGuiKey_Backspace;
    case Platform::Input::Key::Tab:
        return ImGuiKey_Tab;
    case Platform::Input::Key::PageUp:
        return ImGuiKey_PageUp;
    case Platform::Input::Key::PageDown:
        return ImGuiKey_PageDown;
    case Platform::Input::Key::End:
        return ImGuiKey_End;
    case Platform::Input::Key::Home:
        return ImGuiKey_Home;
    case Platform::Input::Key::Insert:
        return ImGuiKey_Insert;
    case Platform::Input::Key::Delete:
        return ImGuiKey_Delete;
    case Platform::Input::Key::Add:
        return ImGuiKey_KeypadAdd;
    case Platform::Input::Key::Subtract:
        return ImGuiKey_KeypadSubtract;
    case Platform::Input::Key::Multiply:
        return ImGuiKey_KeypadMultiply;
    case Platform::Input::Key::Divide:
        return ImGuiKey_KeypadDivide;
    case Platform::Input::Key::Left:
        return ImGuiKey_LeftArrow;
    case Platform::Input::Key::Right:
        return ImGuiKey_RightArrow;
    case Platform::Input::Key::Up:
        return ImGuiKey_UpArrow;
    case Platform::Input::Key::Down:
        return ImGuiKey_DownArrow;
    case Platform::Input::Key::Numpad0:
        return ImGuiKey_Keypad0;
    case Platform::Input::Key::Numpad1:
        return ImGuiKey_Keypad1;
    case Platform::Input::Key::Numpad2:
        return ImGuiKey_Keypad2;
    case Platform::Input::Key::Numpad3:
        return ImGuiKey_Keypad3;
    case Platform::Input::Key::Numpad4:
        return ImGuiKey_Keypad4;
    case Platform::Input::Key::Numpad5:
        return ImGuiKey_Keypad5;
    case Platform::Input::Key::Numpad6:
        return ImGuiKey_Keypad6;
    case Platform::Input::Key::Numpad7:
        return ImGuiKey_Keypad7;
    case Platform::Input::Key::Numpad8:
        return ImGuiKey_Keypad8;
    case Platform::Input::Key::Numpad9:
        return ImGuiKey_Keypad9;
    case Platform::Input::Key::F1:
        return ImGuiKey_F1;
    case Platform::Input::Key::F2:
        return ImGuiKey_F2;
    case Platform::Input::Key::F3:
        return ImGuiKey_F3;
    case Platform::Input::Key::F4:
        return ImGuiKey_F4;
    case Platform::Input::Key::F5:
        return ImGuiKey_F5;
    case Platform::Input::Key::F6:
        return ImGuiKey_F6;
    case Platform::Input::Key::F7:
        return ImGuiKey_F7;
    case Platform::Input::Key::F8:
        return ImGuiKey_F8;
    case Platform::Input::Key::F9:
        return ImGuiKey_F9;
    case Platform::Input::Key::F10:
        return ImGuiKey_F10;
    case Platform::Input::Key::F11:
        return ImGuiKey_F11;
    case Platform::Input::Key::F12:
        return ImGuiKey_F12;
    case Platform::Input::Key::Pause:
        return ImGuiKey_Pause;

    default:
        return ImGuiKey_None;
    };
}

static constexpr int convertMouseButton(Platform::Input::MouseButton button)
{
    switch (button)
    {
    case Platform::Input::MouseLeft:
        return 0;

    case Platform::Input::MouseRight:
        return 1;

    case Platform::Input::MouseMiddle:
        return 2;

    default:
        return -1;
    }
}

// TODO(pgm) ImGui Rendering...

// IO Handling

// TODO(pgm) Required MouseDownEvent

bool GUISystem::onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        return false;
    }

    io.AddMouseButtonEvent(convertMouseButton(mouseDownEvent.button), true);
    return true;
}

bool GUISystem::onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        return false;
    }

    io.AddMouseButtonEvent(convertMouseButton(mouseUpEvent.button), true);
    return true;
}

bool GUISystem::onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard)
    {
        return false;
    }

    io.AddKeyEvent(convertToImGuiKey(keyDownEvent.key), true);
    return true;
}

bool GUISystem::onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard)
    {
        return false;
    }

    io.AddKeyEvent(convertToImGuiKey(keyUpEvent.key), false);
    return true;
}

// Life cycle
void GUISystem::onActivate()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
}

void GUISystem::onDeactivate()
{
    ImGui::DestroyContext();
}

// Actual logic
void GUISystem::beginFrame()
{
    ImGuiIO &io = ImGui::GetIO();

    const auto wnd = m_App.window();
    io.DisplaySize.x = static_cast<float>(wnd->width());
    io.DisplaySize.y = static_cast<float>(wnd->height());

    ImGui::NewFrame();
}

void GUISystem::endFrame()
{
    ImGui::Render();
}

void GUISystem::onUpdate(const Timespan &deltaTime)
{
    ImGui::ShowDemoWindow();
}

} // namespace PGM::GUI