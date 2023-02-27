#include <PGM/Application/Application.h>
#include <PGM/ECS/Components/Components.h>
#include <PGM/ECS/Entity/EntityRef.h>
#include <PGM/ECS/Scene/Scene.h>
#include <PGM/Renderer/API/Backends.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

class SandboxSystem : public PGM::ApplicationSystem
{
  public:
    inline SandboxSystem(const PGM::Application &app) : PGM::ApplicationSystem(app)
    {
        m_Scene.createEntity("Entity 1");
        m_Scene.createEntity("Entity 2");
    }

    // Actual logic
    void beginFrame() override
    {
        const auto window = m_App.window();
        m_App.context()->setViewport({0, 0, window->width(), window->height()});
        m_App.context()->setClipRegion({0, 0, window->width(), window->height()});
        m_App.context()->clear(PGM::Renderer::API::bColor | PGM::Renderer::API::bDepth, PGM::Color{m_Red, 0, 0, 1});
    }

    void endFrame() override
    {
    }

    void onUpdate(const PGM::Timespan &deltaTime) override
    {
        if (m_Red < 1.0f)
        {
            float deltaRed = deltaTime.seconds() / 5.0f;
            m_Red += deltaRed;
            m_Red = std::min(m_Red, 1.0f);
        }

        if (ImGui::Begin("Scene"))
        {
            m_Scene.for_each([](PGM::ECS::EntityRef entity) {
                auto &tag = entity.getComponent<PGM::ECS::Components::TagComponent>();
                ImGui::Selectable(tag.name.c_str());
            });
        }
        ImGui::End();
    }

  private:
    PGM::ECS::Scene m_Scene;
    float m_Red{0.0f};
};

int main()
{
    PGM::Application app = PGM::Application::create<PGM::Renderer::OpenGL>(
        PGM::window_creation_args_t{"PGM Sandbox"},
        PGM::Renderer::OPENGL_DEFAULT_CONTEXT_FLAGS // | PGM::Renderer::bOglContextCompatibiltyMode
    );
    app.pushSystem<SandboxSystem>();
    app.run();

    return 0;
}