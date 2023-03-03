#pragma once

#include "OpenGlCommands.h"

#include <PGM/Core/Ref/Ref.h>
#include <PGM/Platform/Window/Window.h>

namespace PGM::OpenGL
{

enum OpenGlContextFlags
{
    bOglContextCompatibiltyMode = 0b0001
};

using OpenGlContextFlagCreationMask = int;

static constexpr OpenGlContextFlagCreationMask OPENGL_DEFAULT_CONTEXT_FLAGS = 0;

class OpenGlRenderContext
{
  public:
    OpenGlRenderContext(SharedRef<Window> window, OpenGlContextFlagCreationMask flags = OPENGL_DEFAULT_CONTEXT_FLAGS,
                        int majorVersion = 4, int minorVersion = 6);

    ~OpenGlRenderContext();

    OpenGlRenderContext(OpenGlRenderContext &&) noexcept;
    OpenGlRenderContext &operator=(OpenGlRenderContext &&) noexcept;

    bool bind() const;
    void unbind() const;
    void swapBuffers() const;

    inline const RendererCommands *commands() const
    {
        return &m_Commands;
    }

    inline SharedRef<Window> window() const
    {
        return m_Window;
    }

  private:
    struct context_t;

    SharedRef<Window> m_Window;
    Ref<context_t, void (*)(context_t *)> m_Context;
    OpenGlCommands m_Commands;
};

} // namespace PGM::OpenGL