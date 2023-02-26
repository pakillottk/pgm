#include <PGM/Application/Application.h>
#include <PGM/Renderer/API/Backends.h>

int main()
{
    PGM::Application app = PGM::Application::create<PGM::Renderer::OpenGL>(
        PGM::window_creation_args_t{"PGM Sandbox"},
        PGM::Renderer::OPENGL_DEFAULT_CONTEXT_FLAGS // | PGM::Renderer::bOglContextCompatibiltyMode
    );
    app.run();

    return 0;
}