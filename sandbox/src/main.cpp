#include <PGM/Application/Application.h>
#include <PGM/Renderer/API/Backends.h>

class SandboxSystem : public PGM::ApplicationSystem
{
  public:
    constexpr SandboxSystem(const PGM::Application &app) : PGM::ApplicationSystem(app)
    {
    }

    // IO Handling
    bool onMouseMove(const PGM::Platform::WindowEvents::MouseMove &mouseMoveEvent) override
    {
        return false;
    }

    bool onMouseDown(const PGM::Platform::WindowEvents::MouseButtonDown &mouseDownEvent) override
    {
        return false;
    }
    bool onMouseUp(const PGM::Platform::WindowEvents::MouseButtonUp &mouseUpEvent) override
    {
        return false;
    }

    bool onKeyDown(const PGM::Platform::WindowEvents::WindowKeyDown &keyDownEvent) override
    {
        return false;
    }
    bool onKeyUp(const PGM::Platform::WindowEvents::WindowKeyUp &keyUpEvent) override
    {
        return false;
    }

    // Life cycle
    void onActivate() override
    {
    }

    void onDeactivate() override
    {
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
    }

  private:
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