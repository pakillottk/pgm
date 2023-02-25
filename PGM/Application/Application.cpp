#include "Application.h"

#include <PGM/Core/Logging/Logger.h>
#include <PGM/Renderer/API/Backends.h>

// Force dedicated GPU
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace PGM
{

void Application::run()
{
    m_Window->show();

    const int DURATION = 1000;
    const float DELTA = 1.0f / DURATION;
    size_t frame = 0;
    float t = 0.0f;
    int dir = 1;
    while (m_Window->pumpMessages())
    {
        // TODO: FPS Cap
        std::scoped_lock<std::mutex> lk{m_RendererMutex};
        if (!m_RenderContext.bind())
        {
            break;
        }

        m_RenderContext->setViewport({0, 0, m_Window->width(), m_Window->height()});
        // m_RenderContext->clear(Renderer::API::bColor | Renderer::API::bDepth, Colors::Black);

        auto buff = m_RenderContext->createBuffer(false, sizeof(int));
        const int data = 4;
        buff->write(0, 4);

        auto dynBuff = m_RenderContext->createBuffer(true, sizeof(int));
        dynBuff->write(0, data);
        dynBuff->commit();

        buff->destroy();
        dynBuff->destroy();

        Color clearColor{t, t, t, 1.0f};
        m_RenderContext->clear(Renderer::API::bColor | Renderer::API::bDepth, clearColor);

        m_RenderContext.swapBuffers();
        m_RenderContext.unbind();

        if ((dir > 0 && t >= 1.0f) || (dir < 0 && t <= 0.0f))
        {
            dir *= -1;
        }
        t += DELTA * dir;
        ++frame;
    }
}

void Application::onWindowResized(const Platform::WindowEvents::WindowResizedEvent &resizeEvent)
{
    Logging::log_debug("Window resized: W={} H={}", resizeEvent.width, resizeEvent.height);
}

void Application::onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent)
{
    Logging::log_debug("Mouse button pressed: {} (Double clicked: {})", mouseDownEvent.button,
                       mouseDownEvent.isDoubleClick);
}

void Application::onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent)
{
    Logging::log_debug("Mouse button released: {}", mouseUpEvent.button);
}

void Application::onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent)
{
    Logging::log_debug("Key pressed: {}", keyDownEvent.key);
    if (keyDownEvent.key == Platform::Input::Enter && Platform::Input::isKeyDown(Platform::Input::LAlt))
    {
        m_Window->setFullScreen(!m_Window->isFullScreen());
    }
}

void Application::onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent)
{
    Logging::log_debug("Key released: {}", keyUpEvent.key);
}

void Application::bindEvents()
{
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::WindowResizedEvent>(
        [this](const auto &event) { this->onWindowResized(event); }));

    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::MouseButtonDown>(
        [this](const auto &event) { this->onMouseDown(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::MouseButtonUp>(
        [this](const auto &event) { this->onMouseUp(event); }));

    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::WindowKeyDown>(
        [this](const auto &event) { this->onKeyDown(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::WindowKeyUp>(
        [this](const auto &event) { this->onKeyUp(event); }));
}

} // namespace PGM