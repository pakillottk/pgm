#include "GUISystem.h"
#include "../Application.h"

#include "../../Platform/Input/Keyboard.h"
#include "../../Platform/Input/Mouse.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace PGM::GUI
{

static constexpr ImGuiKey convertToImGuiKey(Input::Key key)
{
    switch (key)
    {
    case Input::Key::A:
        return ImGuiKey_A;
    case Input::Key::B:
        return ImGuiKey_B;
    case Input::Key::C:
        return ImGuiKey_C;
    case Input::Key::D:
        return ImGuiKey_D;
    case Input::Key::E:
        return ImGuiKey_E;
    case Input::Key::F:
        return ImGuiKey_F;
    case Input::Key::G:
        return ImGuiKey_G;
    case Input::Key::H:
        return ImGuiKey_H;
    case Input::Key::I:
        return ImGuiKey_I;
    case Input::Key::J:
        return ImGuiKey_J;
    case Input::Key::K:
        return ImGuiKey_K;
    case Input::Key::L:
        return ImGuiKey_L;
    case Input::Key::M:
        return ImGuiKey_M;
    case Input::Key::N:
        return ImGuiKey_N;
    case Input::Key::O:
        return ImGuiKey_O;
    case Input::Key::P:
        return ImGuiKey_P;
    case Input::Key::Q:
        return ImGuiKey_Q;
    case Input::Key::R:
        return ImGuiKey_R;
    case Input::Key::S:
        return ImGuiKey_S;
    case Input::Key::T:
        return ImGuiKey_T;
    case Input::Key::U:
        return ImGuiKey_U;
    case Input::Key::V:
        return ImGuiKey_V;
    case Input::Key::W:
        return ImGuiKey_W;
    case Input::Key::X:
        return ImGuiKey_X;
    case Input::Key::Y:
        return ImGuiKey_Y;
    case Input::Key::Z:
        return ImGuiKey_Z;

    case Input::Key::Num0:
        return ImGuiKey_0;
    case Input::Key::Num1:
        return ImGuiKey_1;
    case Input::Key::Num2:
        return ImGuiKey_2;
    case Input::Key::Num3:
        return ImGuiKey_3;
    case Input::Key::Num4:
        return ImGuiKey_4;
    case Input::Key::Num5:
        return ImGuiKey_5;
    case Input::Key::Num6:
        return ImGuiKey_6;
    case Input::Key::Num7:
        return ImGuiKey_7;
    case Input::Key::Num8:
        return ImGuiKey_8;
    case Input::Key::Num9:
        return ImGuiKey_9;
    case Input::Key::Escape:
        return ImGuiKey_Escape;
    case Input::Key::LControl:
        return ImGuiKey_LeftCtrl;
    case Input::Key::LShift:
        return ImGuiKey_LeftShift;
    case Input::Key::LAlt:
        return ImGuiKey_LeftAlt;
    case Input::Key::LSystem:
        return ImGuiKey_LeftSuper;
    case Input::Key::RControl:
        return ImGuiKey_RightCtrl;
    case Input::Key::RShift:
        return ImGuiKey_RightShift;
    case Input::Key::RAlt:
        return ImGuiKey_RightAlt;
    case Input::Key::RSystem:
        return ImGuiKey_RightSuper;
    case Input::Key::Menu:
        return ImGuiKey_Menu;
    case Input::Key::LBracket:
        return ImGuiKey_LeftBracket;
    case Input::Key::RBracket:
        return ImGuiKey_RightBracket;
    case Input::Key::Semicolon:
        return ImGuiKey_Semicolon;
    case Input::Key::Comma:
        return ImGuiKey_Comma;
    case Input::Key::Period:
        return ImGuiKey_Period;
    case Input::Key::Apostrophe:
        return ImGuiKey_Apostrophe;
    case Input::Key::Slash:
        return ImGuiKey_Slash;
    case Input::Key::Backslash:
        return ImGuiKey_Backslash;
    case Input::Key::Grave:
        return ImGuiKey_GraveAccent;
    case Input::Key::Equal:
        return ImGuiKey_Equal;
    case Input::Key::Space:
        return ImGuiKey_Space;
    case Input::Key::Enter:
        return ImGuiKey_Enter;
    case Input::Key::Backspace:
        return ImGuiKey_Backspace;
    case Input::Key::Tab:
        return ImGuiKey_Tab;
    case Input::Key::PageUp:
        return ImGuiKey_PageUp;
    case Input::Key::PageDown:
        return ImGuiKey_PageDown;
    case Input::Key::End:
        return ImGuiKey_End;
    case Input::Key::Home:
        return ImGuiKey_Home;
    case Input::Key::Insert:
        return ImGuiKey_Insert;
    case Input::Key::Delete:
        return ImGuiKey_Delete;
    case Input::Key::Add:
        return ImGuiKey_KeypadAdd;
    case Input::Key::Subtract:
        return ImGuiKey_KeypadSubtract;
    case Input::Key::Multiply:
        return ImGuiKey_KeypadMultiply;
    case Input::Key::Divide:
        return ImGuiKey_KeypadDivide;
    case Input::Key::Left:
        return ImGuiKey_LeftArrow;
    case Input::Key::Right:
        return ImGuiKey_RightArrow;
    case Input::Key::Up:
        return ImGuiKey_UpArrow;
    case Input::Key::Down:
        return ImGuiKey_DownArrow;
    case Input::Key::Numpad0:
        return ImGuiKey_Keypad0;
    case Input::Key::Numpad1:
        return ImGuiKey_Keypad1;
    case Input::Key::Numpad2:
        return ImGuiKey_Keypad2;
    case Input::Key::Numpad3:
        return ImGuiKey_Keypad3;
    case Input::Key::Numpad4:
        return ImGuiKey_Keypad4;
    case Input::Key::Numpad5:
        return ImGuiKey_Keypad5;
    case Input::Key::Numpad6:
        return ImGuiKey_Keypad6;
    case Input::Key::Numpad7:
        return ImGuiKey_Keypad7;
    case Input::Key::Numpad8:
        return ImGuiKey_Keypad8;
    case Input::Key::Numpad9:
        return ImGuiKey_Keypad9;
    case Input::Key::F1:
        return ImGuiKey_F1;
    case Input::Key::F2:
        return ImGuiKey_F2;
    case Input::Key::F3:
        return ImGuiKey_F3;
    case Input::Key::F4:
        return ImGuiKey_F4;
    case Input::Key::F5:
        return ImGuiKey_F5;
    case Input::Key::F6:
        return ImGuiKey_F6;
    case Input::Key::F7:
        return ImGuiKey_F7;
    case Input::Key::F8:
        return ImGuiKey_F8;
    case Input::Key::F9:
        return ImGuiKey_F9;
    case Input::Key::F10:
        return ImGuiKey_F10;
    case Input::Key::F11:
        return ImGuiKey_F11;
    case Input::Key::F12:
        return ImGuiKey_F12;
    case Input::Key::Pause:
        return ImGuiKey_Pause;

    default:
        return ImGuiKey_None;
    };
}

static constexpr int convertMouseButton(Input::MouseButton button)
{
    switch (button)
    {
    case Input::MouseLeft:
        return 0;

    case Input::MouseRight:
        return 1;

    case Input::MouseMiddle:
        return 2;

    default:
        return -1;
    }
}

// IO Handling

bool GUISystem::onMouseMove(const WindowEvents::MouseMove &mouseMoveEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent(mouseMoveEvent.location.x, mouseMoveEvent.location.y);
    return io.WantCaptureMouse;
}

bool GUISystem::onMouseDown(const WindowEvents::MouseButtonDown &mouseDownEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        return false;
    }

    io.AddMouseButtonEvent(convertMouseButton(mouseDownEvent.button), true);
    return true;
}

bool GUISystem::onMouseUp(const WindowEvents::MouseButtonUp &mouseUpEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        return false;
    }

    io.AddMouseButtonEvent(convertMouseButton(mouseUpEvent.button), false);
    return true;
}

bool GUISystem::onKeyDown(const WindowEvents::WindowKeyDown &keyDownEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard)
    {
        return false;
    }

    if (!keyDownEvent.repeat)
    {
        io.AddKeyEvent(convertToImGuiKey(keyDownEvent.key), true);
    }

    return true;
}

bool GUISystem::onKeyUp(const WindowEvents::WindowKeyUp &keyUpEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard)
    {
        return false;
    }

    io.AddKeyEvent(convertToImGuiKey(keyUpEvent.key), false);
    return true;
}

bool GUISystem::onTextInput(const WindowEvents::WindowTextInput &textInputEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.WantTextInput)
    {
        return false;
    }

    io.AddInputCharactersUTF8(textInputEvent.input.c_str());
    return true;
}

// Life cycle
void GUISystem::onActivate()
{
    IMGUI_CHECKVERSION();
    auto ctx = ImGui::CreateContext();
    ImGuizmo::SetImGuiContext(ctx);

    // TODO(pgm) These flags are not correct for all the platforms, but for now we only support win32...
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendFlags |=
        ImGuiBackendFlags_PlatformHasViewports; // We can create multi-viewports on the Platform side (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can call io.AddMouseViewportEvent() with correct
                                                                  // data (optional)

    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    mainViewport->PlatformHandle = m_App.window().get();

    m_RenderData = GUI::initializeRenderData(m_App.renderer());

    ImGui::StyleColorsDark();
    setupImGuiStyle();
}

void GUISystem::onDeactivate()
{
    ImGui::DestroyContext();
    destroyRenderData(m_RenderData);
}

// Actual logic
void GUISystem::beginFrame()
{
    ImGuiIO &io = ImGui::GetIO();

    const auto wnd = m_App.window();
    io.DisplaySize.x = static_cast<float>(wnd->width());
    io.DisplaySize.y = static_cast<float>(wnd->height());

    ImGui::NewFrame();

    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}

void GUISystem::endFrame()
{
    ImGui::Render();
    GUI::renderImGui(ImGui::GetDrawData(), m_RenderData);
}

void GUISystem::onUpdate(const Timespan &deltaTime)
{
    ImGui::GetIO().DeltaTime = deltaTime.seconds();
    // ImGui::ShowDemoWindow();
}

} // namespace PGM::GUI