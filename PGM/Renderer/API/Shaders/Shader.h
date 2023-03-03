#pragma once

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Matrix/Matrix.h>
#include <PGM/Core/Vec/Vec.h>

#include <string_view>

namespace PGM
{

class Shader
{
  public:
    virtual ~Shader() = default;

    virtual void destroy() = 0;

    virtual operator bool() const = 0;

    virtual int id() const = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual int getUniformLocation(const std::string_view &id) const = 0;

    virtual void setUniform(int loc, int value) const = 0;

    virtual void setUniform(int loc, float value) const = 0;
    virtual void setUniform(int loc, const Vec2 &value) const = 0;
    virtual void setUniform(int loc, const Vec3 &value) const = 0;
    virtual void setUniform(int loc, const Vec4 &value) const = 0;

    virtual void setUniform(int loc, const Mat4 &matrix) const = 0;
};

} // namespace PGM