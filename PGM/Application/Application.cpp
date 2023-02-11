#include "Application.h"

#include <PGM/Core/Logging/Logger.h>
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

void Application::onWindowResized(const Platform::WindowEvents::WindowResizedEvent &resizeEvent)
{
    Logging::log_info("Window resized: W={} H={}", resizeEvent.width, resizeEvent.height);
}

void Application::bindEvents()
{
    m_EventListeners.push_back(m_Window->dispatcher().suscribe<Platform::WindowEvents::WindowResizedEvent>(
        [this](const auto &event) { this->onWindowResized(event); }));
}

} // namespace PGM