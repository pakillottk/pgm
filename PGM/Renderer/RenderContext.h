#pragma once

#include "API/RendererCommands.h"

#include "../Core/Ref/Ref.h"
#include "../Platform/Window/Window.h"

#include <memory>

namespace PGM
{

class RenderContext
{
  public:
    template <typename RenderContextBackend>
    explicit RenderContext(RenderContextBackend backend)
        : m_Context{std::make_unique<graphics_context_model_t<RenderContextBackend>>(std::move(backend))}
    {
    }

    RenderContext(RenderContext &&) = default;
    RenderContext &operator=(RenderContext &&) = default;

    inline bool bind() const
    {
        return m_Context->bind();
    }

    inline void unbind() const
    {
        m_Context->unbind();
    }

    inline void swapBuffers() const
    {
        m_Context->swapBuffers();
    }

    inline const RendererCommands *commands() const
    {
        return m_Context->commands();
    }

    inline const RendererCommands *operator->() const
    {
        return commands();
    }

    inline SharedRef<Window> window() const
    {
        return m_Context->window();
    }

  private:
    struct graphics_context_concept_t
    {
        virtual ~graphics_context_concept_t() = default;

        virtual bool bind() const = 0;
        virtual void unbind() const = 0;
        virtual void swapBuffers() const = 0;
        virtual const RendererCommands *commands() const = 0;

        virtual SharedRef<Window> window() const = 0;
    };

    template <typename RenderContextBackend> struct graphics_context_model_t : public graphics_context_concept_t
    {
        inline graphics_context_model_t(RenderContextBackend _backend) : backend{std::move(_backend)}
        {
        }

        inline bool bind() const override
        {
            return backend.bind();
        }

        inline void unbind() const override
        {
            backend.unbind();
        }

        virtual const RendererCommands *commands() const override
        {
            return backend.commands();
        }

        virtual void swapBuffers() const override
        {
            backend.swapBuffers();
        }

        inline SharedRef<Window> window() const override
        {
            return backend.window();
        }

        RenderContextBackend backend;
    };

    Ref<graphics_context_concept_t> m_Context;
};

} // namespace PGM