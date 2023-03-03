#pragma once

#include "../Core/Time/Time.h"
#include "../Platform/Window/Events/WindowEvents.h"
#include "../Platform/Window/Window.h"

namespace PGM
{

class Application;

class ApplicationSystem
{
  public:
    constexpr ApplicationSystem(Application &app) : m_App(app)
    {
    }

    // IO Handling
    inline virtual bool onMouseMove([[maybe_unused]] const WindowEvents::MouseMove &mouseMoveEvent)
    {
        return false;
    }
    inline virtual bool onMouseDown([[maybe_unused]] const WindowEvents::MouseButtonDown &mouseDownEvent)
    {
        return false;
    }

    inline virtual bool onMouseUp([[maybe_unused]] const WindowEvents::MouseButtonUp &mouseUpEvent)
    {
        return false;
    }

    inline virtual bool onKeyDown([[maybe_unused]] const WindowEvents::WindowKeyDown &keyDownEvent)
    {
        return false;
    }

    inline virtual bool onKeyUp([[maybe_unused]] const WindowEvents::WindowKeyUp &keyUpEvent)
    {
        return false;
    }

    inline virtual bool onTextInput([[maybe_unused]] const WindowEvents::WindowTextInput &textInputEvent)
    {
        return false;
    }

    // Life cycle
    virtual void onActivate()
    {
    }
    virtual void onDeactivate()
    {
    }

    // Actual logic
    virtual void beginFrame()
    {
    }

    virtual void endFrame()
    {
    }

    virtual void onUpdate([[maybe_unused]] const Timespan &deltaTime)
    {
    }

  protected:
    Application &m_App;
};

} // namespace PGM