#pragma once

#include "../../Core/Events/EventDispatcher.h"
#include "../../Core/Rect/Rect.h"
#include "../../Core/Ref/Ref.h"

#include <string_view>

namespace PGM
{

/**
 * Creates a new window handle which abstracts
 * from the concrete platform windowing
 */
class Window
{
  public:
    enum FLAGS
    {
        bDefault = 0
        // TODO
    };

    Window(const std::string_view &title, unsigned w = 800, unsigned h = 600, FLAGS flags = bDefault);
    ~Window();

    /// @brief Bottom left corner (as most graphics API coord system)
    /// @return
    RectInt rect() const;
    int width() const;
    int height() const;

    void show();

    inline Events::EventQueue *dispatcher()
    {
        return &m_Dispatcher;
    }

    inline bool isFullScreen() const
    {
        return m_FullScreen;
    }

    void setFullScreen(bool v);

    /**
     * Runs the window's main loop
     */
    bool pumpMessages();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    struct window_impl_t;
    const window_impl_t &impl() const;

  private:
    Ref<window_impl_t> m_Impl;
    Events::EventDispatcher m_Dispatcher;

    bool m_FullScreen;
};

} // namespace PGM