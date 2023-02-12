#pragma once

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Ref/Ref.h>
#include <PGM/Platform/Window/Events/WindowEvents.h>
#include <PGM/Platform/Window/Window.h>
#include <PGM/Renderer/RenderContext.h>

#include <mutex>
#include <vector>

namespace PGM
{

struct window_creation_args_t
{
    std::string title{"PGM App"};
    int width{800};
    int height{600};
    // TODO
};

class Application
{
  public:
    virtual ~Application() = default;

    template <typename RenderContextBackend, typename... Args>
    inline static Application create(const window_creation_args_t &wndArgs, Args &&...args)
    {
        auto wnd = std::make_shared<Platform::Window>(wndArgs.title, wndArgs.width, wndArgs.height);
        return Application{wnd, createContext<RenderContextBackend>(wnd, std::forward<Args>(args)...)};
    }

    template <typename RenderContextBackend> inline static Application create(const window_creation_args_t &wndArgs)
    {
        auto wnd = std::make_shared<Platform::Window>(wndArgs.title, wndArgs.width, wndArgs.height);
        return Application(wnd, createContext<RenderContextBackend>(wnd));
    }

    /// @brief Swaps the render context. Can be done while running
    template <typename RenderContextBackend, typename... Args> inline void setContext(Args &&...args)
    {
        std::scoped_lock<std::mutex> lk{m_RendererMutex};
        m_RenderContext = createContext<RenderContextBackend>(m_Window, std::forward<Args>(args)...);
    }

    /// @brief Swaps the render context. Can be done while running
    template <typename RenderContextBackend> inline void setContext()
    {
        std::scoped_lock<std::mutex> lk{m_RendererMutex};
        m_RenderContext = createContext<RenderContextBackend>(m_Window);
    }

    const Renderer::RenderContext &context() const
    {
        return m_RenderContext;
    }

    void run();

  protected:
    virtual void onWindowResized(const Platform::WindowEvents::WindowResizedEvent &resizeEvent);
    virtual void onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent);
    virtual void onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent);

  private:
    void bindEvents();

    template <typename RenderContextBackend, typename... Args>
    static inline Renderer::RenderContext createContext(SharedRef<Platform::Window> wnd, Args &&...args)
    {
        Logging::log_debug("(APP) Creating render context with backend: {}", typeid(RenderContextBackend).name());
        return Renderer::RenderContext(RenderContextBackend(wnd, std::forward<Args>(args)...));
    }

    template <typename RenderContextBackend>
    static inline Renderer::RenderContext createContext(SharedRef<Platform::Window> wnd)
    {
        Logging::log_debug("(APP) Creating render context with backend: {}", typeid(RenderContextBackend).name());
        return Renderer::RenderContext(RenderContextBackend(wnd));
    }

    inline Application(SharedRef<Platform::Window> wnd, Renderer::RenderContext context)
        : m_Window{wnd}, m_RenderContext{std::move(context)}
    {
        bindEvents();
    }

    SharedRef<Platform::Window> m_Window;
    Renderer::RenderContext m_RenderContext;

    std::mutex m_RendererMutex;
    std::vector<Events::EventListener> m_EventListeners;
};

} // namespace PGM