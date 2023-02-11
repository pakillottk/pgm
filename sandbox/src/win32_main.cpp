#include <Windows.h>

#include <PGM/Application/Application.h>
#include <PGM/Renderer/API/Backends.h>

// int CALLBACK WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
//                      [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
int main()
{
    PGM::Application app = PGM::Application::create<PGM::Renderer::OpenGL>(PGM::window_creation_args_t{"PGM Sandbox"});
    app.run();

    return 0;
}