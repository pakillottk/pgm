#pragma once

#include "../ApplicationSystem.h"

#include "../../Renderer/API/Buffers/VertexArray.h"
#include "../../Renderer/API/Shaders/Shader.h"
#include "../../Renderer/API/Textures/Texture2d.h"

#include "../../GUI/GUI.h"

namespace PGM::GUI
{

class GUISystem : public PGM::ApplicationSystem
{
  public:
    inline GUISystem(Application &app) : ApplicationSystem(app)
    {
    }

    // IO Handling
    bool onMouseMove(const WindowEvents::MouseMove &mouseMoveEvent) override;
    bool onMouseDown(const WindowEvents::MouseButtonDown &mouseDownEvent) override;
    bool onMouseUp(const WindowEvents::MouseButtonUp &mouseUpEvent) override;

    bool onKeyDown(const WindowEvents::WindowKeyDown &keyDownEvent) override;
    bool onKeyUp(const WindowEvents::WindowKeyUp &keyUpEvent) override;
    bool onTextInput(const WindowEvents::WindowTextInput &textInputEvent) override;

    // Life cycle
    void onActivate() override;
    void onDeactivate() override;

    // Actual logic
    void beginFrame() override;
    void endFrame() override;
    void onUpdate(const Timespan &deltaTime) override;

  private:
    GUI::RenderData m_RenderData;
};

} // namespace PGM