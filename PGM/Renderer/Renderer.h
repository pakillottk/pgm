#pragma once

#include "RenderContext.h"

#include <vector>

namespace PGM
{

class Renderer
{
  public:
    inline Renderer(RenderContext ctx) : m_Context{std::move(ctx)}
    {
    }

    inline void setRenderContext(RenderContext ctx)
    {
        m_Context = std::move(ctx);
    }

    [[nodiscard]] constexpr const RenderContext &context() const noexcept
    {
        return m_Context;
    }

    inline bool begin() const
    {
        return m_Context.bind();
    }

    inline void end() const
    {
        m_Context.swapBuffers();
        m_Context.unbind();
    }

    inline const RendererCommands *operator->() const
    {
        return m_Context.commands();
    }

  private:
    RenderContext m_Context;
};

} // namespace PGM