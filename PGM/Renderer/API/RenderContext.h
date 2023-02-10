#include "Commands.h"

#include <PGM/Core/Ref/Ref.h>
#include <PGM/Platform/Window/Window.h>

#include <memory>

namespace PGM::Renderer::API
{
class RenderContext
{
  public:
    template <typename RenderContextBackend>
    RenderContext(RenderContextBackend backend)
        : m_Context{std::make_unique<graphics_context_model_t<RenderContextBackend>>(std::move(backend))}
    {
    }

    inline void bind() const
    {
        m_Context->bind();
    }

    inline void unbind() const
    {
        m_Context->unbind();
    }

    inline void swapBuffers() const
    {
        m_Context->swapBuffers();
    }

    inline const Commands *commands() const
    {
        return m_Context->commands();
    }

    inline const Commands *operator->() const
    {
        return commands();
    }

    inline SharedRef<Platform::Window> window() const
    {
        return m_Context->window();
    }

  private:
    struct graphics_context_concept_t
    {
        virtual ~graphics_context_concept_t() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void swapBuffers() const = 0;
        virtual const Commands *commands() const = 0;

        virtual SharedRef<Platform::Window> window() const = 0;
    };

    template <typename RenderContextBackend> struct graphics_context_model_t : public graphics_context_concept_t
    {
        inline graphics_context_model_t(RenderContextBackend _backend) : backend{std::move(_backend)}
        {
        }

        inline void bind() const override
        {
            backend.bind();
        }

        inline void unbind() const override
        {
            backend.unbind();
        }

        virtual const Commands *commands() const override
        {
            return backend.commands();
        }

        virtual void swapBuffers() const override
        {
            backend.swapBuffers();
        }

        inline SharedRef<Platform::Window> window() const override
        {
            return backend.window();
        }

        RenderContextBackend backend;
    };

    Ref<graphics_context_concept_t> m_Context;
};

} // namespace PGM::Renderer::API