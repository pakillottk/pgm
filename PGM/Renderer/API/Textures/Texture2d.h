#pragma once

namespace PGM::Renderer::API::Textures
{

enum PixelType
{
    Byte,
    Int,
    Float
};

// TODO(pgm) Wrapping mode, mipmap config...

class Texture2d
{
  public:
    virtual ~Texture2d() = default;

    virtual void destroy() = 0;

    virtual operator bool() const = 0;
    virtual int id() const = 0;

    virtual PixelType pixelType() const = 0;
    virtual unsigned channels() const = 0;

    virtual unsigned width() const = 0;
    virtual unsigned height() const = 0;
    virtual void write(unsigned x, unsigned y, unsigned w, unsigned h, const void *data) = 0;

    virtual void bind(int unit) const = 0;
    virtual void unbind() const = 0;
};

} // namespace PGM::Renderer::API::Textures