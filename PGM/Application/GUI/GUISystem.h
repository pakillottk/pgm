#pragma once

#include "../ApplicationSystem.h"

#include <PGM/Renderer/API/Buffers/VertexArray.h>
#include <PGM/Renderer/API/Shaders/Shader.h>
#include <PGM/Renderer/API/Textures/Texture2d.h>

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

    // Life cycle
    void onActivate() override;
    void onDeactivate() override;

    // Actual logic
    void beginFrame() override;
    void endFrame() override;
    void onUpdate(const Timespan &deltaTime) override;

    struct RenderData
    {
        SharedRef<PGM::Renderer::API::Shaders::Shader> shader;

        SharedRef<PGM::Renderer::API::Textures::Texture2d> fontAtlas;

        SharedRef<PGM::Renderer::API::Buffers::GpuBuffer> vertices;
        SharedRef<PGM::Renderer::API::Buffers::GpuBuffer> indices;

        SharedRef<PGM::Renderer::API::Buffers::VertexArray> vao;

        int projMatLoc;
        int textSamplerLoc;
    };

  private:
    RenderData m_RenderData;
};

} // namespace PGM::GUI