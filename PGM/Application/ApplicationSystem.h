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
    inline virtual bool onMouseMove(const WindowEvents::MouseMove &mouseMoveEvent)
    {
        return false;
    }
    inline virtual bool onMouseDown(const WindowEvents::MouseButtonDown &mouseDownEvent)
    {
        return false;
    }

    inline virtual bool onMouseUp(const WindowEvents::MouseButtonUp &mouseUpEvent)
    {
        return false;
    }

    inline virtual bool onKeyDown(const WindowEvents::WindowKeyDown &keyDownEvent)
    {
        return false;
    }

    inline virtual bool onKeyUp(const WindowEvents::WindowKeyUp &keyUpEvent)
    {
        return false;
    }

    inline virtual bool onTextInput(const WindowEvents::WindowTextInput &textInputEvent)
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

    virtual void onUpdate(const Timespan &deltaTime)
    {
    }

  protected:
    Application &m_App;
};

} // namespace PGM