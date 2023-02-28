#pragma once

#include "../ApplicationSystem.h"

#include <PGM/Renderer/API/Buffers/VertexArray.h>
#include <PGM/Renderer/API/Shaders/Shader.h>
#include <PGM/Renderer/API/Textures/Texture2d.h>

#include <PGM/GUI/GUI.h>

namespace PGM::GUI
{

class GUISystem : public ApplicationSystem
{
  public:
    inline GUISystem(const Application &app) : ApplicationSystem(app)
    {
    }

    // IO Handling
    bool onMouseMove(const Platform::WindowEvents::MouseMove &mouseMoveEvent) override;
    bool onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent) override;
    bool onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent) override;

    bool onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent) override;
    bool onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent) override;
    bool onTextInput(const Platform::WindowEvents::WindowTextInput &textInputEvent) override;

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

} // namespace PGM::GUI