#include "OpenGlShader.h"
#include "../GlCheck.h"

#include <PGM/Core/Logging/Logger.h>
#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

namespace PGM::Renderer::API::Backend::OpenGL::Shaders
{

static int createShader(const std::string_view &vertexSource, const std::string_view &fragmentSource)
{
    const int id = glCreateProgram();

    GLint status{};

    // Vertex Shader
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSrcPtr = vertexSource.data();
    glShaderSource(vertexShader, 1, &vertexSrcPtr, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        PGM_ASSERT(status == GL_TRUE, "Vertex Shader compilation failed");

        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, buffer);
        PGM::Logging::log_error("Shader compilation error: {}", buffer);

        glDeleteShader(vertexShader);

        return 0;
    }

    // Fragment Shader
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSrcPtr = fragmentSource.data();
    glShaderSource(fragmentShader, 1, &fragmentSrcPtr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        PGM_ASSERT(status == GL_TRUE, "Fragment Shader compilation failed");

        char buffer[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        PGM::Logging::log_error("Shader compilation error: {}", buffer);

        glDeleteShader(fragmentShader);

        return 0;
    }

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    glValidateProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        PGM_ASSERT(status == GL_TRUE, "Program link failed");

        char buffer[512];
        glGetProgramInfoLog(id, 512, NULL, buffer);
        PGM::Logging::log_error("Shader compilation error: {}", buffer);

        glDeleteProgram(id);

        return 0;
    }

    PGM_CHECK_GL();

    return id;
}

OpenGlShader::OpenGlShader(const std::string_view &vertexSource, const std::string_view &fragmentSource)
{
    m_Id = createShader(vertexSource, fragmentSource);
    PGM_ASSERT(m_Id > 0, "Invalid shader program");
}

void OpenGlShader::destroy()
{
    if (m_Id > 0)
    {
        glDeleteProgram(m_Id);
        m_Id = 0;
    }
}

OpenGlShader::operator bool() const
{
    return m_Id > 0;
}

int OpenGlShader::id() const
{
    return m_Id;
}

void OpenGlShader::bind() const
{
    PGM_ASSERT(m_Id > 0, "Invalid shader program");

    glUseProgram(m_Id);
    PGM_CHECK_GL();
}

void OpenGlShader::unbind() const
{
    glUseProgram(0);
    PGM_CHECK_GL();
}

int OpenGlShader::getUniformLocation(const std::string_view &id) const
{
    int loc = glGetUniformLocation(m_Id, id.data());
    PGM_CHECK_GL();

    return loc;
}

void OpenGlShader::setUniform(int loc, int value) const
{
    bind();

    glUniform1i(loc, value);
    PGM_CHECK_GL();
}

void OpenGlShader::setUniform(int loc, float value) const
{
    bind();

    glUniform1f(loc, value);
    PGM_CHECK_GL();
}

void OpenGlShader::setUniform(int loc, const Vec2 &value) const
{
    bind();

    glUniform2f(loc, value.x, value.y);
    PGM_CHECK_GL();
}

void OpenGlShader::setUniform(int loc, const Vec3 &value) const
{
    bind();

    glUniform3f(loc, value.x, value.y, value.z);
    PGM_CHECK_GL();
}

void OpenGlShader::setUniform(int loc, const Vec4 &value) const
{
    bind();

    glUniform4f(loc, value.x, value.y, value.z, value.w);
    PGM_CHECK_GL();
}

void OpenGlShader::setUniform(int loc, const Mat4 &matrix) const
{
    bind();

    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    PGM_CHECK_GL();
}

} // namespace PGM::Renderer::API::Backend::OpenGL::Shaders