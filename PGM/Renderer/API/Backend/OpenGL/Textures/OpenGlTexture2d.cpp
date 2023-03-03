#include "OpenGlTexture2d.h"
#include "../GlCheck.h"

#include "../../../../../Core/Assert/Assert.h"
#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

namespace PGM::OpenGL
{

static constexpr GLenum mapInputFormat(unsigned channelCount)
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

static constexpr GLenum mapTextureInternalFormat(PixelType pixelType, unsigned channelCount)
{
    switch (pixelType)
    {
    case PGM::BytePixel:
        switch (channelCount)
        {
        case 1:
            return GL_R;

        case 2:
            return GL_RG;

        case 3:
            return GL_RGB;

        case 4:
            return GL_RGBA;
        }
        break;

    case PGM::IntPixel:
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

    case PGM::FloatPixel:
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

static constexpr GLenum mapPixelDataType(PixelType pixelType)
{
    switch (pixelType)
    {
    case PGM::BytePixel:
        return GL_UNSIGNED_BYTE;

    case PGM::IntPixel:
        return GL_INT;

    case PGM::FloatPixel:
        return GL_FLOAT;

    default:
        PGM_ASSERT(false, "Invalid pixel type");
        return 0;
    }
}

OpenGlTexture2d::OpenGlTexture2d(PixelType pixelType, unsigned channelCount, unsigned w, unsigned h,
                                 const void *data /*= nullptr*/)
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

    const auto internalFormat = mapTextureInternalFormat(pixelType, channelCount);
    const auto format = mapInputFormat(channelCount);
    const auto type = mapPixelDataType(pixelType);
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

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, mapInputFormat(m_ChannelCount), mapPixelDataType(m_PixelType), data);
    PGM_CHECK_GL();

    glBindTexture(GL_TEXTURE_2D, 0);
    PGM_CHECK_GL();
}

void OpenGlTexture2d::bind(int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    PGM_CHECK_GL();

    glBindTexture(GL_TEXTURE_2D, m_Id);
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

} // namespace PGM::OpenGL