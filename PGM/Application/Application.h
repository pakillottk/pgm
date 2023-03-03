#pragma once

#include "ApplicationSystemStack.h"
#include "GUI/GUISystem.h"

#include "../Core/Assert/Assert.h"
#include "../Core/Ref/Ref.h"
#include "../Core/Time/Time.h"
#include "../Platform/Window/Events/WindowEvents.h"
#include "../Platform/Window/Window.h"
#include "../Renderer/Renderer.h"

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
        auto wnd = std::make_shared<Window>(wndArgs.title, wndArgs.width, wndArgs.height);
        return Application{wnd, createContext<RenderContextBackend>(wnd, std::forward<Args>(args)...)};
    }

    template <typename RenderContextBackend> inline static Application create(const window_creation_args_t &wndArgs)
    {
        auto wnd = std::make_shared<Window>(wndArgs.title, wndArgs.width, wndArgs.height);
        return Application(wnd, createContext<RenderContextBackend>(wnd));
    }

    /// @brief Swaps the render context. Can be done while running
    template <typename RenderContextBackend, typename... Args> inline void setContext(Args &&...args)
    {
        m_Renderer = Renderer{createContext<RenderContextBackend>(m_Window, std::forward<Args>(args)...)};
    }

    /// @brief Swaps the render context. Can be done while running
    template <typename RenderContextBackend> inline void setContext()
    {
        m_Renderer = Renderer{createContext<RenderContextBackend>(m_Window)};
    }

    constexpr Renderer &renderer()
    {
        return m_Renderer;
    }

    template <typename SystemType, typename... Args> inline void pushSystem(Args &&...args)
    {
        m_SystemsStack.push<SystemType>(*this, std::forward<Args>(args)...);
    }

    inline const SharedRef<Window> window() const
    {
        return m_Window;
    }

    void run();

  protected:
    void onWindowClose(const WindowEvents::WindowClose &closeEvent);
    void onWindowResized(const WindowEvents::WindowResized &resizeEvent);

    void onMouseMove(const WindowEvents::MouseMove &mouseMoveEvent);
    void onMouseDown(const WindowEvents::MouseButtonDown &mouseDownEvent);
    void onMouseUp(const WindowEvents::MouseButtonUp &mouseUpEvent);

    void onKeyDown(const WindowEvents::WindowKeyDown &keyDownEvent);
    void onKeyUp(const WindowEvents::WindowKeyUp &keyUpEvent);
    void onTextInput(const WindowEvents::WindowTextInput &textInputEvent);

  private:
    void bindEvents();

    template <typename RenderContextBackend, typename... Args>
    static inline RenderContext createContext(SharedRef<Window> wnd, Args &&...args)
    {
        Logging::log_debug("(APP) Creating render context with backend: {}", typeid(RenderContextBackend).name());
        return RenderContext(RenderContextBackend(wnd, std::forward<Args>(args)...));
    }

    template <typename RenderContextBackend> static inline RenderContext createContext(SharedRef<Window> wnd)
    {
        Logging::log_debug("(APP) Creating render context with backend: {}", typeid(RenderContextBackend).name());
        return RenderContext(RenderContextBackend(wnd));
    }

    inline Application(SharedRef<Window> wnd, RenderContext context)
        : m_Window{wnd}, m_Renderer{std::move(context)}, m_WindowClosed{false}, m_GUI(*this)
    {
        bindEvents();
    }

    SharedRef<Window> m_Window;
    GUI::GUISystem m_GUI;
    ApplicationSystemStack m_SystemsStack;
    Renderer m_Renderer;
    bool m_WindowClosed;

    Time m_LastFrameTime;
    std::vector<Events::EventListener> m_EventListeners;
};

} // namespace PGM