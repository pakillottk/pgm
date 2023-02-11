#pragma once

#include "OpenGlCommands.h"

#include <PGM/Core/Ref/Ref.h>
#include <PGM/Platform/Window/Window.h>

namespace PGM::Renderer
{

enum OpenGlContextFlags
{
    bOglContextCompatibiltyMode = 0b0001
};

using OpenGlContextFlagCreationMask = int;

static constexpr OpenGlContextFlagCreationMask OPENGL_DEFAULT_CONTEXT_FLAGS = 0;

} // namespace PGM::Renderer

namespace PGM::Renderer::API::Backend
{

class OpenGlRenderContext
{
  public:
    OpenGlRenderContext(SharedRef<Platform::Window> window,
                        OpenGlContextFlagCreationMask flags = OPENGL_DEFAULT_CONTEXT_FLAGS, int majorVersion = 4,
                        int minorVersion = 6);

    ~OpenGlRenderContext();

    OpenGlRenderContext(OpenGlRenderContext &&) noexcept;
    OpenGlRenderContext &operator=(OpenGlRenderContext &&) noexcept;

    bool bind() const;
    void unbind() const;
    void swapBuffers() const;

    inline const Commands *commands() const
    {
        return &m_Commands;
    }

    inline SharedRef<Platform::Window> window() const
    {
        return m_Window;
    }

  private:
    struct context_t;

    SharedRef<Platform::Window> m_Window;
    Ref<context_t, void (*)(context_t *)> m_Context;
    OpenGlCommands m_Commands;
};

} // namespace PGM::Renderer::API::Backend