#pragma once

#include "../ApplicationSystem.h"

namespace PGM::GUI
{

class GUISystem : public ApplicationSystem
{
  public:
    constexpr GUISystem(const Application &app) : ApplicationSystem(app)
    {
    }

    // IO Handling
    bool onMouseDown(const Platform::WindowEvents::MouseButtonDown &mouseDownEvent) override;
    bool onMouseUp(const Platform::WindowEvents::MouseButtonUp &mouseUpEvent) override;

    bool onKeyDown(const Platform::WindowEvents::WindowKeyDown &keyDownEvent) override;
    bool onKeyUp(const Platform::WindowEvents::WindowKeyUp &keyUpEvent) override;

    // Life cycle
    void onActivate() override;
    void onDeactivate() override;

    // Actual logic
    void beginFrame() override;
    void endFrame() override;
    void onUpdate(const Timespan &deltaTime) override;
};

}