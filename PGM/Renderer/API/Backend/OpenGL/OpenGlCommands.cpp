#include "OpenGlCommands.h"

#include "Buffers/GpuBufferTraits.h"
#include "Buffers/VertexArrayImpl.h"
#include "GlCheck.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

#include <string>

#define internal static

namespace PGM::Renderer::API::Backend
{

void OpenGlCommands::clear(ClearBufferMask mask, Color clearColor /*= Colors::Black*/) const
{
    GLbitfield glMask{0};
    if ((mask & bColor) > 0)
    {
        glMask |= GL_COLOR_BUFFER_BIT;
    }
    if ((mask & bDepth) > 0)
    {
        glMask |= GL_DEPTH_BUFFER_BIT;
    }
    if ((mask & bStencil) > 0)
    {
        glMask |= GL_STENCIL_BUFFER_BIT;
    }

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(glMask);

    PGM_CHECK_GL();
}

void OpenGlCommands::setViewport(const RectInt &viewportRect) const
{
    glEnable(GL_SCISSOR_TEST);

    glScissor(static_cast<GLint>(viewportRect.x), static_cast<GLint>(viewportRect.y),
              static_cast<GLsizei>(viewportRect.width), static_cast<GLsizei>(viewportRect.height));

    glViewport(static_cast<GLint>(viewportRect.x), static_cast<GLint>(viewportRect.y),
               static_cast<GLsizei>(viewportRect.width), static_cast<GLsizei>(viewportRect.height));

    PGM_CHECK_GL();
}

SharedRef<Buffers::GpuBuffer> OpenGlCommands::createBuffer(bool dynamic, size_t size,
                                                           const void *data /*= nullptr*/) const
{
    if (dynamic)
    {
        return make_shared_ref<Buffers::GpuBuffer>(OpenGL::Buffers::OpenGlDynamicBufferTraits{}, size, data);
    }
    else
    {
        return make_shared_ref<Buffers::GpuBuffer>(OpenGL::Buffers::OpenGlStaticBufferTraits{}, size, data);
    }
}

SharedRef<Buffers::VertexArray> OpenGlCommands::createVertexArray(
    std::initializer_list<Buffers::VertexAttrib> attributes) const
{
    return make_shared_ref<OpenGL::Buffers::VertexArrayImpl>(attributes);
}

SharedRef<Buffers::VertexArray> OpenGlCommands::createIndexedVertexArray(
    const Buffers::VertexAttrib &indexAttribute, std::initializer_list<Buffers::VertexAttrib> attributes) const
{
    return make_shared_ref<OpenGL::Buffers::VertexArrayImpl>(indexAttribute, attributes);
}

} // namespace PGM::Renderer::API::Backend