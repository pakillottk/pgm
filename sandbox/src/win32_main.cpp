#include <Windows.h>

#include <PGM/Core/Ref/Ref.h>
#include <PGM/Platform/Window/Window.h>
#include <PGM/Renderer/API/Backend/OpenGL/OpenGlRenderContext.h>
#include <PGM/Renderer/API/RenderContext.h>

int CALLBACK WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                     [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
    PGM::SharedRef<PGM::Platform::Window> wnd = std::make_shared<PGM::Platform::Window>("PGM - Sandbox");
    PGM::Renderer::API::RenderContext ctx{PGM::Renderer::API::Backend::OpenGlRenderContext{wnd}};

    wnd->show();
    ctx.bind();

    while (wnd->pumpMessages())
    {
        ctx->clear(PGM::Renderer::API::bColor | PGM::Renderer::API::bDepth, PGM::Colors::Red);
        ctx.swapBuffers();
    }

    return 0;
}