#include "OpenGlCommands.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <gl/GL.h>

#include <string>

#define internal static

namespace PGM::Renderer::API::Backend
{

internal inline GLenum glCheckError(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
            // case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        Logging::log_error("(OpenGL) Error: {}\tSource: {}:{}", error, file, line);
    }
    return errorCode;
}
#ifdef PGM_ASSERTS_ENABLED
#define PGM_CHECK_GL() PGM_ASSERT(glCheckError(__FILE__, __LINE__) == GL_NO_ERROR, "OpenGL Error");
#else
#define PGM_CHECK_GL() glCheckError(__FILE__, __LINE__)
#endif

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

} // namespace PGM::Renderer::API::Backend