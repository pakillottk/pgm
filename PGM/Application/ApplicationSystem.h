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
    // IO Handling
    virtual bool onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent) = 0;
    virtual bool onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent) = 0;

    virtual bool onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent) = 0;
    virtual bool onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent) = 0;

    // Life cycle
    virtual void onActivate(const Application &app) = 0;
    virtual void onDeactivate(const Application &app) = 0;

    // Actual logic
    virtual void beginFrame(const Application &app) = 0;
    virtual void endFrame(const Application &app) = 0;
    virtual void onUpdate(const Application &app, const Timespan &deltaTime) = 0;
};

} // namespace PGM