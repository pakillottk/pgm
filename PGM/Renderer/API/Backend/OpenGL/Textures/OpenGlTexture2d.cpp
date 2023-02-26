#include "OpenGlTexture2d.h"
#include "../GlCheck.h"

#include <PGM/Core/Assert/Assert.h>
#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

namespace PGM::Renderer::API::Backend::OpenGL::Textures
{

static constexpr GLenum mapInternalFormat(unsigned channelCount)
{
    switch (channelCount)
    {
    case 1:
        return GL_RED;

    case 2:
        return GL_RG;

    case 3:
        return GL_RGB;

    case 4:
        return GL_RGBA;

    default:
        return 0;
    }
}

static constexpr GLenum mapPixelFormat(PGM::Renderer::API::Textures::PixelType pixelType, unsigned channelCount)
{
    switch (pixelType)
    {
    case PGM::Renderer::API::Textures::Byte:
        switch (channelCount)
        {
        case 1:
            return GL_R8;

        case 2:
            return GL_RG8;

        case 3:
            return GL_RGB8;

        case 4:
            return GL_RGBA8;
        }
        break;

    case PGM::Renderer::API::Textures::Int:
        switch (channelCount)
        {
        case 1:
            return GL_R32I;

        case 2:
            return GL_RG32I;

        case 3:
            return GL_RGB32I;

        case 4:
            return GL_RGB32I;
        }
        break;

    case PGM::Renderer::API::Textures::Float:
        switch (channelCount)
        {
        case 1:
            return GL_R32F;

        case 2:
            return GL_RG32F;

        case 3:
            return GL_RGB32F;

        case 4:
            return GL_RGB32F;
        }
        break;

    default:
        PGM_ASSERT(false, "Invalid pixel type");
        return 0;
    }

    return 0;
}

static constexpr GLenum mapType(PGM::Renderer::API::Textures::PixelType pixelType)
{
    switch (pixelType)
    {
    case PGM::Renderer::API::Textures::Byte:
        return GL_UNSIGNED_BYTE;

    case PGM::Renderer::API::Textures::Int:
        return GL_INT;

    case PGM::Renderer::API::Textures::Float:
        return GL_FLOAT;

    default:
        PGM_ASSERT(false, "Invalid pixel type");
        return 0;
    }
}

OpenGlTexture2d::OpenGlTexture2d(PGM::Renderer::API::Textures::PixelType pixelType, unsigned channelCount, unsigned w,
                                 unsigned h, const void *data /*= nullptr*/)
    : m_Id(0), m_PixelType(pixelType), m_ChannelCount(channelCount), m_Width(w), m_Height(h)
{
    PGM_ASSERT(channelCount > 0 && channelCount <= 4, "Channel count must be [1-4]");

    GLuint texId;
    glGenTextures(1, &texId);
    PGM_CHECK_GL();

    if (texId == 0)
    {
        return;
    }

    m_Id = static_cast<int>(texId);

    glBindTexture(GL_TEXTURE_2D, texId);
    PGM_CHECK_GL();

    // TODO(pgm) This should be configurable...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    PGM_CHECK_GL();

    const auto internalFormat = mapInternalFormat(channelCount);
    const auto format = mapPixelFormat(pixelType, channelCount);
    const auto type = mapType(pixelType);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, data);
    PGM_CHECK_GL();

    glBindTexture(GL_TEXTURE_2D, 0);
    PGM_CHECK_GL();
}

void OpenGlTexture2d::write(unsigned x, unsigned y, unsigned w, unsigned h, const void *data)
{
    PGM_ASSERT(m_Id > 0, "Invalid texture");
    PGM_ASSERT((x + w) <= m_Width, "Out of bounds");
    PGM_ASSERT((x + h) <= m_Height, "Out of bounds");

    glBindTexture(GL_TEXTURE_2D, m_Id);
    PGM_CHECK_GL();

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, mapPixelFormat(m_PixelType, m_ChannelCount), mapType(m_PixelType),
                    data);
    PGM_CHECK_GL();

    glBindTexture(GL_TEXTURE_2D, 0);
    PGM_CHECK_GL();
}

void OpenGlTexture2d::bind(int unit) const
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
    PGM_CHECK_GL();

    glActiveTexture(GL_TEXTURE0 + unit);
    PGM_CHECK_GL();
}

void OpenGlTexture2d::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    PGM_CHECK_GL();
}

void OpenGlTexture2d::destroyImpl()
{
    if (m_Id == 0)
    {
        return;
    }

    const GLuint texId = m_Id;
    glDeleteTextures(1, &texId);
    m_Id = 0;
}

} // namespace PGM::Renderer::API::Backend::OpenGL::Textures