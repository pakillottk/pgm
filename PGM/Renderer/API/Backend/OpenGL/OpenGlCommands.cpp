#include "OpenGlCommands.h"

#include "Buffers/GpuBufferTraits.h"
#include "Buffers/VertexArrayImpl.h"
#include "GlCheck.h"
#include "Shaders/OpenGlShader.h"
#include "Textures/OpenGlTexture2d.h"

#include "../../../../Core/Assert/Assert.h"
#include "../../../../Core/Logging/Logger.h"

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

#include <string>

#define internal static

namespace PGM::OpenGL
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

SharedRef<GpuBuffer> OpenGlCommands::createBuffer(bool dynamic, size_t size, const void *data /*= nullptr*/) const
{
    if (dynamic)
    {
        return make_shared_ref<GpuBuffer>(OpenGL::OpenGlDynamicBufferTraits{}, size, data);
    }
    else
    {
        return make_shared_ref<GpuBuffer>(OpenGL::OpenGlStaticBufferTraits{}, size, data);
    }
}

SharedRef<VertexArray> OpenGlCommands::createVertexArray(std::initializer_list<VertexAttrib> attributes) const
{
    return make_shared_ref<OpenGL::VertexArrayImpl>(attributes);
}

SharedRef<VertexArray> OpenGlCommands::createIndexedVertexArray(const VertexAttrib &indexAttribute,
                                                                std::initializer_list<VertexAttrib> attributes) const
{
    return make_shared_ref<OpenGL::VertexArrayImpl>(indexAttribute, attributes);
}

SharedRef<Shader> OpenGlCommands::createShader(const std::string_view &vertexSource,
                                               const std::string_view &fragmentSource) const
{
    return make_shared_ref<OpenGlShader>(vertexSource, fragmentSource);
}

SharedRef<Texture2d> OpenGlCommands::createTexture2d(PixelType pixelType, unsigned channelCount, unsigned w, unsigned h,
                                                     const void *data /*= nullptr*/) const
{
    return make_shared_ref<OpenGlTexture2d>(pixelType, channelCount, w, h, data);
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

static constexpr GLenum mapIndexType(VertexAttribDataType type)
{
    switch (type)
    {
    case Byte:
        return GL_BYTE;

    case UnsignedByte:
        return GL_UNSIGNED_BYTE;

    case Short:
        return GL_SHORT;

    case UnsignedShort:
        return GL_UNSIGNED_SHORT;

    case Int:
        return GL_INT;

    case Uint:
        return GL_UNSIGNED_INT;

    default:
        PGM_ASSERT(false, "Unknown vertex attrib data type");
        return 0;
    }
}

void OpenGlCommands::drawIndexed(PrimitiveType primitive, unsigned elements, VertexAttribDataType indexType,
                                 size_t offset) const
{
    static_assert(sizeof(size_t) == sizeof(void *), "Size mismatch");
    glDrawElements(mapPrimitiveType(primitive), static_cast<GLsizei>(elements), mapIndexType(indexType),
                   reinterpret_cast<void *>(offset));
    PGM_CHECK_GL();
}

void OpenGlCommands::draw(PrimitiveType primitive, unsigned vertexCount, size_t offset) const
{
    glDrawArrays(mapPrimitiveType(primitive), static_cast<GLint>(offset), static_cast<GLint>(vertexCount));
    PGM_CHECK_GL();
}

} // namespace PGM::OpenGL