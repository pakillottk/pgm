#include "Application.h"

#include <PGM/Core/Logging/Logger.h>
#include <PGM/Renderer/API/Backends.h>

#include <array>
#include <cstddef>

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

    m_LastFrameTime = Timer::now();

    while (m_Window->pumpMessages())
    {
        if (m_WindowClosed)
        {
            break;
        }

        // TODO: FPS Cap
        std::scoped_lock<std::mutex> lk{m_RendererMutex};
        if (!m_RenderContext.bind())
        {
            break;
        }

        const auto time = Timer::now();
        const Timespan deltaTime{m_LastFrameTime, time};
        m_LastFrameTime = time;

        for (auto &system : m_SystemsStack)
        {
            system->beginFrame(*this);
        }

        for (auto &system : m_SystemsStack)
        {
            system->onUpdate(*this, deltaTime);
        }

        for (auto &system : m_SystemsStack)
        {
            system->endFrame(*this);
        }

        m_RenderContext.swapBuffers();
        m_RenderContext.unbind();
    }
}

void Application::onWindowClose([[maybe_unused]] const Platform::WindowEvents::WindowClose &closeEvent)
{
    m_WindowClosed = true;
    Logging::log_debug("Window closed");
}

void Application::onWindowResized(const Platform::WindowEvents::WindowResizedEvent &resizeEvent)
{
    Logging::log_debug("Window resized: W={} H={}", resizeEvent.width, resizeEvent.height);
}

void Application::onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent)
{
    Logging::log_debug("Mouse button pressed: {} (Double clicked: {})", mouseDownEvent.button,
                       mouseDownEvent.isDoubleClick);

    for (auto &system : m_SystemsStack)
    {
        if (system->onMouseDown(mouseDownEvent))
        {
            break;
        }
    }
}

void Application::onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent)
{
    Logging::log_debug("Mouse button released: {}", mouseUpEvent.button);

    for (auto &system : m_SystemsStack)
    {
        if (system->onMouseUp(mouseUpEvent))
        {
            break;
        }
    }
}

void Application::onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent)
{
    Logging::log_debug("Key pressed: {}", keyDownEvent.key);

    for (auto &system : m_SystemsStack)
    {
        if (system->onKeyDown(keyDownEvent))
        {
            break;
        }
    }
}

void Application::onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent)
{
    Logging::log_debug("Key released: {}", keyUpEvent.key);

    for (auto &system : m_SystemsStack)
    {
        if (system->onKeyUp(keyUpEvent))
        {
            break;
        }
    }
}

void Application::bindEvents()
{
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<Platform::WindowEvents::WindowClose>(
        [this](const auto &event) { this->onWindowClose(event); }));

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