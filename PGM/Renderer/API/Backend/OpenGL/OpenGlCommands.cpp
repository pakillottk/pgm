#include "OpenGlCommands.h"

#include "Buffers/GpuBufferTraits.h"
#include "Buffers/VertexArrayImpl.h"
#include "GlCheck.h"
#include "Shaders/OpenGlShader.h"
#include "Textures/OpenGlTexture2d.h"

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

    glViewport(static_cast<GLint>(viewportRect.x), static_cast<GLint>(viewportRect.y),
               static_cast<GLsizei>(viewportRect.width), static_cast<GLsizei>(viewportRect.height));

    PGM_CHECK_GL();
}

void OpenGlCommands::setClipRegion(const RectInt &clipRect) const
{
    glEnable(GL_SCISSOR_TEST);

    glScissor(static_cast<GLint>(clipRect.x), static_cast<GLint>(clipRect.y), static_cast<GLsizei>(clipRect.width),
              static_cast<GLsizei>(clipRect.height));

    PGM_CHECK_GL();
}

void OpenGlCommands::depthTest(bool enable) const
{
    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    PGM_CHECK_GL();
}

void OpenGlCommands::blending(bool enable) const
{
    if (enable)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
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

SharedRef<Shaders::Shader> OpenGlCommands::createShader(const std::string_view &vertexSource,
                                                        const std::string_view &fragmentSource) const
{
    return make_shared_ref<OpenGL::Shaders::OpenGlShader>(vertexSource, fragmentSource);
}

SharedRef<Textures::Texture2d> OpenGlCommands::createTexture2d(Textures::PixelType pixelType, unsigned channelCount,
                                                               unsigned w, unsigned h,
                                                               const void *data /*= nullptr*/) const
{
    return make_shared_ref<Backend::OpenGL::Textures::OpenGlTexture2d>(pixelType, channelCount, w, h, data);
}

static constexpr GLenum mapPrimitiveType(PrimitiveType primitive)
{
    switch (primitive)
    {
    case Triangles:
        return GL_TRIANGLES;

    default:
        PGM_ASSERT(false, "Unknown primitive");
        return 0;
    }
}

static constexpr GLenum mapIndexType(Buffers::VertexAttribDataType type)
{
    switch (type)
    {
    case Buffers::Byte:
        return GL_BYTE;

    case Buffers::UnsignedByte:
        return GL_UNSIGNED_BYTE;

    case Buffers::Short:
        return GL_SHORT;

    case Buffers::UnsignedShort:
        return GL_UNSIGNED_SHORT;

    case Buffers::Int:
        return GL_INT;

    case Buffers::Uint:
        return GL_UNSIGNED_INT;

    default:
        PGM_ASSERT(false, "Unknown vertex attrib data type");
        return 0;
    }
}

void OpenGlCommands::drawIndexed(PrimitiveType primitive, unsigned elements, Buffers::VertexAttribDataType indexType,
                                 size_t offset) const
{
    static_assert(sizeof(size_t) == sizeof(void *), "Size mismatch");
    glDrawElements(mapPrimitiveType(primitive), static_cast<GLsizei>(elements), mapIndexType(indexType),
                   reinterpret_cast<void *>(offset));
    PGM_CHECK_GL();
}

} // namespace PGM::Renderer::API::Backend