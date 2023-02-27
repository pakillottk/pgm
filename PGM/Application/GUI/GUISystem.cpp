#include "GUISystem.h"
#include "../Application.h"

#include <PGM/Platform/Input/Keyboard.h>
#include <PGM/Platform/Input/Mouse.h>

// Compile IMGUI
#include <imgui/imgui.cpp>
#include <imgui/imgui.h>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_tables.cpp>
#include <imgui/imgui_widgets.cpp>

// Methods for std string
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace ImGui
{
struct InputTextCallback_UserData
{
    std::string *Str;
    ImGuiInputTextCallback ChainCallback;
    void *ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData *data)
{
    InputTextCallback_UserData *user_data = (InputTextCallback_UserData *)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back
        // to what we want.
        std::string *str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char *)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool ImGui::InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
                      void *user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputText(label, (char *)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool ImGui::InputTextMultiline(const char *label, std::string *str, const ImVec2 &size, ImGuiInputTextFlags flags,
                               ImGuiInputTextCallback callback, void *user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextMultiline(label, (char *)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback,
                              &cb_user_data);
}

bool ImGui::InputTextWithHint(const char *label, const char *hint, std::string *str, ImGuiInputTextFlags flags,
                              ImGuiInputTextCallback callback, void *user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextWithHint(label, hint, (char *)str->c_str(), str->capacity() + 1, flags, InputTextCallback,
                             &cb_user_data);
}

} // namespace ImGui

// IMGUI Demo
#include <imgui/imgui_demo.cpp>

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

static inline void initializeRenderData(const Renderer::API::Commands &api, GUISystem::RenderData &renderData)
{
    static constexpr const char *vertex_shader_glsl_410_core = "#version 430 core\n"
                                                               "layout (location = 0) in vec2 Position;\n"
                                                               "layout (location = 1) in vec2 UV;\n"
                                                               "layout (location = 2) in vec4 Color;\n"
                                                               "uniform mat4 ProjMtx;\n"
                                                               "out vec2 Frag_UV;\n"
                                                               "out vec4 Frag_Color;\n"
                                                               "void main()\n"
                                                               "{\n"
                                                               "    Frag_UV = UV;\n"
                                                               "    Frag_Color = Color;\n"
                                                               "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                                                               "}\n";

    static constexpr const char *fragment_shader_glsl_410_core =
        "#version 430 core\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "uniform sampler2D Texture;\n"
        "layout (location = 0) out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    static constexpr unsigned POSITION_LOC = 0;
    static constexpr unsigned UV_LOC = 1;
    static constexpr unsigned COLOR_LOC = 2;

    renderData.shader = api.createShader(vertex_shader_glsl_410_core, fragment_shader_glsl_410_core);

    renderData.projMatLoc = renderData.shader->getUniformLocation("ProjMtx");
    renderData.textSamplerLoc = renderData.shader->getUniformLocation("Texture");

    // Create buffers
    renderData.vertices = api.createBuffer(true, 0);
    renderData.indices = api.createBuffer(true, 0);
    renderData.vao = api.createIndexedVertexArray(
        Renderer::API::Buffers::VertexAttrib{renderData.indices},
        {Renderer::API::Buffers::VertexAttrib{renderData.vertices, POSITION_LOC, Renderer::API::Buffers::Float, 2,
                                              sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, pos)},
         Renderer::API::Buffers::VertexAttrib{renderData.vertices, UV_LOC, Renderer::API::Buffers::Float, 2,
                                              sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, uv)},
         Renderer::API::Buffers::VertexAttrib{renderData.vertices, COLOR_LOC, Renderer::API::Buffers::UnsignedByte, 4,
                                              sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, col), true}});

    ImGuiIO &io = ImGui::GetIO();

    // Build texture atlas
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    renderData.fontAtlas = api.createTexture2d(Renderer::API::Textures::Byte, 4, width, height, pixels);

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(renderData.fontAtlas.get()));
}

static void destroyRenderData(GUISystem::RenderData &renderData)
{
    renderData = {};
}

static inline void setupImGuiStyle()
{
    // Material Flat style by ImJC1C from ImThemes
    ImGuiStyle &style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.5f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Left;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.8313725590705872f, 0.8470588326454163f, 0.8784313797950745f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] =
        ImVec4(0.8313725590705872f, 0.8470588326454163f, 0.8784313797950745f, 0.501960813999176f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1725490242242813f, 0.1921568661928177f, 0.2352941185235977f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.1587982773780823f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1725490242242813f, 0.1921568661928177f, 0.2352941185235977f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] =
        ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.501960813999176f);
    style.Colors[ImGuiCol_FrameBgHovered] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] =
        ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.5333333611488342f, 0.5333333611488342f, 0.5333333611488342f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] =
        ImVec4(0.3333333432674408f, 0.3333333432674408f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] =
        ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.239215686917305f, 0.5215686559677124f, 0.8784313797950745f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] =
        ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9803921580314636f, 1.0f);
    style.Colors[ImGuiCol_Button] =
        ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 0.501960813999176f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
    style.Colors[ImGuiCol_SeparatorHovered] =
        ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
    style.Colors[ImGuiCol_SeparatorActive] =
        ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 0.250980406999588f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1529411822557449f, 0.1725490242242813f, 0.2117647081613541f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] =
        ImVec4(0.3098039329051971f, 0.6235294342041016f, 0.9333333373069763f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] =
        ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] =
        ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 0.5021458864212036f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03862661123275757f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] =
        ImVec4(0.2039215713739395f, 0.2313725501298904f, 0.2823529541492462f, 0.7529411911964417f);
    style.Colors[ImGuiCol_NavWindowingDimBg] =
        ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.7529411911964417f);
    style.Colors[ImGuiCol_ModalWindowDimBg] =
        ImVec4(0.105882354080677f, 0.1137254908680916f, 0.1372549086809158f, 0.7529411911964417f);
}

static void renderImGui(ImDrawData *draw_data, const Renderer::API::Commands &api, GUISystem::RenderData &renderData)
{
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
    {
        return;
    }

    api.blending(true);
    api.depthTest(false);
    api.setViewport({0, 0, fb_width, fb_height});

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    Mat4 orthoMat = glm::ortho(L, R, B, T);
    renderData.shader->setUniform(renderData.projMatLoc, orthoMat);
    renderData.shader->setUniform(renderData.textSamplerLoc, 1);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        // Upload vertex/index buffers
        const size_t vtx_buffer_size = cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
        const size_t idx_buffer_size = cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
        renderData.vertices->allocate(vtx_buffer_size, cmd_list->VtxBuffer.Data);
        renderData.indices->allocate(idx_buffer_size, cmd_list->IdxBuffer.Data);

        renderData.shader->bind();
        renderData.vao->bind();
        renderData.fontAtlas->bind(1);

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
        ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

            // Project scissor/clipping rectangles into framebuffer space
            ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                            (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
            ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                            (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
            if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
            {
                continue;
            }
            api.setClipRegion({(int)clip_min.x, (int)((float)fb_height - clip_max.y), (int)(clip_max.x - clip_min.x),
                               (int)(clip_max.y - clip_min.y)});
            api.drawIndexed(Renderer::API::Triangles, pcmd->ElemCount,
                            sizeof(ImDrawIdx) == 2 ? Renderer::API::Buffers::UnsignedShort
                                                   : Renderer::API::Buffers::Uint,
                            (pcmd->IdxOffset * sizeof(ImDrawIdx)));

            // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
            // GL_CALL(glScissor((int)clip_min.x, (int)((float)fb_height - clip_max.y), (int)(clip_max.x - clip_min.x),
            //                   (int)(clip_max.y - clip_min.y)));

            // GL_CALL(glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->GetTexID()));
            // GL_CALL(glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
            //                        sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            //                        (void *)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx))));
        }

        renderData.fontAtlas->unbind();
        renderData.vao->unbind();
        renderData.shader->unbind();
    }
}

// IO Handling

bool GUISystem::onMouseMove(const Platform::WindowEvents::MouseMove &mouseMoveEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent(mouseMoveEvent.location.x, mouseMoveEvent.location.y);
    return io.WantCaptureMouse;
}

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

    io.AddMouseButtonEvent(convertMouseButton(mouseUpEvent.button), false);
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

    initializeRenderData(*m_App.context().commands(), m_RenderData);

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
}

void GUISystem::endFrame()
{
    ImGui::Render();
    renderImGui(ImGui::GetDrawData(), *m_App.context().commands(), m_RenderData);
}

void GUISystem::onUpdate(const Timespan &deltaTime)
{
    ImGui::ShowDemoWindow();
}

} // namespace PGM::GUI