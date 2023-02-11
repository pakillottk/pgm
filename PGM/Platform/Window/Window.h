#pragma once

#include <PGM/Core/Ref/Ref.h>
#include <string_view>

namespace PGM::Platform
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

    void show() const;

    /**
     * Runs the window's main loop
     */
    bool pumpMessages() const;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    struct window_impl_t;
    const window_impl_t &impl() const;

  private:
    Ref<window_impl_t> m_Impl;
};

} // namespace PGM::Platform