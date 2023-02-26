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

    const int DURATION = 1000;
    const float DELTA = 1.0f / DURATION;
    size_t frame = 0;
    float t = 0.0f;
    int dir = 1;
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

        m_RenderContext->setViewport({0, 0, m_Window->width(), m_Window->height()});
        // m_RenderContext->clear(Renderer::API::bColor | Renderer::API::bDepth, Colors::Black);

        auto buff = m_RenderContext->createBuffer(false, sizeof(int));
        const int data = 4;
        buff->write(0, data);

        auto dynBuff = m_RenderContext->createBuffer(true, sizeof(int));
        dynBuff->write(0, data);

        auto vertexArray = m_RenderContext->createIndexedVertexArray(
            PGM::Renderer::API::Buffers::VertexAttrib{buff, PGM::Renderer::API::Buffers::Position,
                                                      PGM::Renderer::API::Buffers::Int, 1, 0, 0},
            {
                PGM::Renderer::API::Buffers::VertexAttrib{dynBuff, PGM::Renderer::API::Buffers::Position,
                                                          PGM::Renderer::API::Buffers::Int, 1, 0, 0},
            });
        vertexArray->bind();
        vertexArray->unbind();

        vertexArray->destroy();

        static const std::string vertexSrc = R"(
            #version 330 core
            
            layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
            
            out vec4 vertexColor; // specify a color output to the fragment shader

            void main()
            {
                gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
                vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
            })";

        static const std::string fragmentSrc = R"(
            #version 330 core

            out vec4 FragColor;
            
            in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

            void main()
            {
                FragColor = vertexColor;
            })";

        auto shader = m_RenderContext->createShader(vertexSrc, fragmentSrc);
        shader->bind();
        shader->unbind();
        shader->destroy();

        static const int pixel = 0xff0000ff;
        auto texture = m_RenderContext->createTexture2d(PGM::Renderer::API::Textures::Byte, 4, 1, 1, &pixel);
        texture->write(0, 0, 1, 1, &pixel);
        texture->bind(0);
        texture->unbind();
        texture->destroy();

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