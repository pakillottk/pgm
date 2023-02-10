#include "OpenGlCommands.h"

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <gl/GL.h>

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
}

} // namespace PGM::Renderer::API::Backend