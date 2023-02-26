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
    virtual bool onMouseMove(const Platform::WindowEvents::MouseMove &mouseMoveEvent) = 0;
    virtual bool onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent) = 0;
    virtual bool onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent) = 0;

    virtual bool onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent) = 0;
    virtual bool onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent) = 0;

    // Life cycle
    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;

    // Actual logic
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void onUpdate(const Timespan &deltaTime) = 0;

  protected:
    const Application &m_App;
};

} // namespace PGM