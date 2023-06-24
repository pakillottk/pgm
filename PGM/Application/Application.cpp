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

    // Initialize GUI
    m_Renderer.begin();
    m_GUI.onActivate();
    m_Renderer.end();

    while (m_Window->pumpMessages())
    {
        if (m_WindowClosed)
        {
            break;
        }

        // TODO: FPS Cap
        if (!m_Renderer.begin())
        {
            break;
        }

        const auto time = Timer::now();
        const Timespan deltaTime{m_LastFrameTime, time};
        m_LastFrameTime = time;

        m_GUI.beginFrame();

        for (auto &system : m_SystemsStack)
        {
            system->beginFrame();
        }

        for (auto &system : m_SystemsStack)
        {
            system->onUpdate(deltaTime);
        }

        m_GUI.onUpdate(deltaTime);

        for (auto &system : m_SystemsStack)
        {
            system->onGui(deltaTime);
        }

        for (auto &system : m_SystemsStack)
        {
            system->endFrame();
        }

        m_Renderer.renderFrame();

        m_GUI.endFrame();
        m_Renderer.end();
    }
}

void Application::onWindowClose([[maybe_unused]] const WindowEvents::WindowClose &closeEvent)
{
    m_WindowClosed = true;
    Logging::log_debug("Window closed");

    m_Renderer.begin();
    m_GUI.onDeactivate();
    m_SystemsStack.clear();
    m_Renderer.end();
}

void Application::onWindowResized(const WindowEvents::WindowResized &resizeEvent)
{
    Logging::log_debug("Window resized: W={} H={}", resizeEvent.width, resizeEvent.height);
}

void Application::onMouseMove(const WindowEvents::MouseMove &mouseMoveEvent)
{
    if (m_GUI.onMouseMove(mouseMoveEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onMouseMove(mouseMoveEvent))
        {
            break;
        }
    }
}

void Application::onMouseDown(const WindowEvents::MouseButtonDown &mouseDownEvent)
{
    Logging::log_debug("Mouse button pressed: {} (Double clicked: {})", mouseDownEvent.button,
                       mouseDownEvent.isDoubleClick);

    if (m_GUI.onMouseDown(mouseDownEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onMouseDown(mouseDownEvent))
        {
            break;
        }
    }
}

void Application::onMouseUp(const WindowEvents::MouseButtonUp &mouseUpEvent)
{
    Logging::log_debug("Mouse button released: {}", mouseUpEvent.button);

    if (m_GUI.onMouseUp(mouseUpEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onMouseUp(mouseUpEvent))
        {
            break;
        }
    }
}

void Application::onKeyDown(const WindowEvents::WindowKeyDown &keyDownEvent)
{
    Logging::log_debug("Key pressed: {} repeat={}", keyDownEvent.key, keyDownEvent.repeat);

    if (m_GUI.onKeyDown(keyDownEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onKeyDown(keyDownEvent))
        {
            break;
        }
    }
}

void Application::onKeyUp(const WindowEvents::WindowKeyUp &keyUpEvent)
{
    Logging::log_debug("Key released: {}", keyUpEvent.key);

    if (m_GUI.onKeyUp(keyUpEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onKeyUp(keyUpEvent))
        {
            break;
        }
    }
}

void Application::onTextInput(const WindowEvents::WindowTextInput &textInputEvent)
{
    Logging::log_debug("Text input received: {}", textInputEvent.input);

    if (m_GUI.onTextInput(textInputEvent))
    {
        return;
    }

    for (auto &system : m_SystemsStack)
    {
        if (system->onTextInput(textInputEvent))
        {
            break;
        }
    }
}

void Application::bindEvents()
{
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::WindowClose>(
        [this](const auto &event) { this->onWindowClose(event); }));

    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::WindowResized>(
        [this](const auto &event) { this->onWindowResized(event); }));

    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::MouseMove>(
        [this](const auto &event) { this->onMouseMove(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::MouseButtonDown>(
        [this](const auto &event) { this->onMouseDown(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::MouseButtonUp>(
        [this](const auto &event) { this->onMouseUp(event); }));

    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::WindowKeyDown>(
        [this](const auto &event) { this->onKeyDown(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::WindowKeyUp>(
        [this](const auto &event) { this->onKeyUp(event); }));
    m_EventListeners.push_back(m_Window->dispatcher()->suscribe<WindowEvents::WindowTextInput>(
        [this](const auto &event) { this->onTextInput(event); }));
}

} // namespace PGM