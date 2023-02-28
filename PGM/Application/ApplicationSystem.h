#pragma once

#include <PGM/Core/Time/Time.h>
#include <PGM/Platform/Window/Events/WindowEvents.h>
#include <PGM/Platform/Window/Window.h>

namespace PGM
{

class Application;

class ApplicationSystem
{
  public:
    constexpr ApplicationSystem(const Application &app) : m_App(app)
    {
    }

    // IO Handling
    inline virtual bool onMouseMove(const Platform::WindowEvents::MouseMove &mouseMoveEvent)
    {
        return false;
    }
    inline virtual bool onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent)
    {
        return false;
    }

    inline virtual bool onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent)
    {
        return false;
    }

    inline virtual bool onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent)
    {
        return false;
    }

    inline virtual bool onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent)
    {
        return false;
    }

    inline virtual bool onTextInput(const Platform::WindowEvents::WindowTextInput &textInputEvent)
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
    const Application &m_App;
};

} // namespace PGM