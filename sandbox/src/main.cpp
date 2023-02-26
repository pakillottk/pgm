#include <PGM/Application/Application.h>
#include <PGM/Renderer/API/Backends.h>

class SandboxSystem : public PGM::ApplicationSystem
{
  public:
    // IO Handling
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
    void onActivate(const PGM::Application &app) override
    {
    }

    void onDeactivate(const PGM::Application &app) override
    {
    }

    // Actual logic
    void beginFrame(const PGM::Application &app) override
    {
        const auto window = app.window();
        app.context()->setViewport({0, 0, window->width(), window->height()});
        app.context()->clear(PGM::Renderer::API::bColor | PGM::Renderer::API::bDepth, PGM::Color{m_Red, 0, 0, 1});
    }

    void endFrame(const PGM::Application &app) override
    {
    }

    void onUpdate(const PGM::Application &app, const PGM::Timespan &deltaTime) override
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