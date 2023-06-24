#pragma once

#include "Batches/StaticBatch.h"
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

    inline void submitBatch(PGM::SharedRef<Shader> shader, PGM::SharedRef<StaticBatch> batch)
    {
        m_Batches.push_back(RendererBatch{shader, batch});
    }

    inline void renderFrame()
    {
        // TODO(pgm) This is a dirty and quick way to to it, until we have materials and better definitions for
        // batches...
        for (const auto &rendererBatch : m_Batches)
        {
            if (rendererBatch.batch->size() == 0)
            {
                continue;
            }

            rendererBatch.shader->bind();

            rendererBatch.batch->vertexArray()->bind();
            m_Context->draw(rendererBatch.batch->primitiveType,
                            static_cast<unsigned>(rendererBatch.batch->vertexCount()), 0);
            rendererBatch.batch->vertexArray()->unbind();

            rendererBatch.shader->unbind();
        }
        m_Batches.clear();
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

    // TODO(pgm) For now... this is a quick dirty way to do it until we define Materials
    struct RendererBatch
    {
        PGM::SharedRef<Shader> shader;
        PGM::SharedRef<StaticBatch> batch;
    };
    mutable std::vector<RendererBatch> m_Batches;
};

} // namespace PGM