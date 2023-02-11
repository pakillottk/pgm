#include "Application.h"

#include <PGM/Renderer/API/Backends.h>

namespace PGM
{

void Application::run()
{
    m_Window->show();
    while (m_Window->pumpMessages())
    {
        // TODO: FPS Cap
        std::scoped_lock<std::mutex> lk{m_RendererMutex};
        if (!m_RenderContext.bind())
        {
            continue;
        }

        m_RenderContext->setViewport({0, 0, m_Window->width(), m_Window->height()});
        m_RenderContext->clear(Renderer::API::bColor | Renderer::API::bDepth, Colors::Black);

        m_RenderContext.swapBuffers();
        m_RenderContext.unbind();
    }
}

} // namespace PGM