#pragma once

#include "../../../Textures/Texture2d.h"

namespace PGM::Renderer::API::Backend::OpenGL::Textures
{

class OpenGlTexture2d : public PGM::Renderer::API::Textures::Texture2d
{
  public:
    OpenGlTexture2d(PGM::Renderer::API::Textures::PixelType pixelType, unsigned channelCount, unsigned w, unsigned h,
                    const void *data = nullptr);

    inline ~OpenGlTexture2d()
    {
        destroyImpl();
    }

    inline void destroy() override
    {
        destroyImpl();
    }

    inline operator bool() const override
    {
        return m_Id > 0;
    }

    inline int id() const override
    {
        return m_Id;
    }

    inline PGM::Renderer::API::Textures::PixelType pixelType() const override
    {
        return m_PixelType;
    }

    unsigned channels() const override
    {
        return m_ChannelCount;
    }

    unsigned width() const override
    {
        return m_Width;
    }

    unsigned height() const override
    {
        return m_Height;
    }

    void write(unsigned x, unsigned y, unsigned w, unsigned h, const void *data) override;

    void bind(int unit) const override;
    void unbind() const override;

  private:
    int m_Id;
    PGM::Renderer::API::Textures::PixelType m_PixelType;
    unsigned m_ChannelCount;
    unsigned m_Width;
    unsigned m_Height;

    void destroyImpl();
};

} // namespace PGM::Renderer::API::Backend::OpenGL::Textures