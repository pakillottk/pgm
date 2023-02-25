#pragma once

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

#include <string>

namespace PGM::Renderer::API::Backend
{

inline GLenum glCheckError(const char *file, int line)
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
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        Logging::log_error("(OpenGL) Error: {}\tSource: {}:{}", error, file, line);
        PGM_ASSERT(errorCode == GL_NO_ERROR, "OpenGL Error");
    }
    return errorCode;
} // namespace PGM::Renderer::API::Backend
#ifdef PGM_ASSERTS_ENABLED
#define PGM_CHECK_GL() PGM_ASSERT(glCheckError(__FILE__, __LINE__) == GL_NO_ERROR, "OpenGL Error");
#else
#define PGM_CHECK_GL() glCheckError(__FILE__, __LINE__)
#endif

} // namespace PGM::Renderer::API::Backend