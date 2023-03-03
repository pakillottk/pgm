#pragma once

#include "../../../Shaders/Shader.h"

namespace PGM::OpenGL
{

class OpenGlShader final : public Shader
{
  public:
    OpenGlShader(const std::string_view &vertexSource, const std::string_view &fragmentSource);

    inline virtual ~OpenGlShader()
    {
        destroy();
    }

    void destroy() override;

    operator bool() const override;

    int id() const override;

    void bind() const override;
    void unbind() const override;

    int getUniformLocation(const std::string_view &id) const override;

    void setUniform(int loc, int value) const override;

    void setUniform(int loc, float value) const override;
    void setUniform(int loc, const Vec2 &value) const override;
    void setUniform(int loc, const Vec3 &value) const override;
    void setUniform(int loc, const Vec4 &value) const override;

    void setUniform(int loc, const Mat4 &matrix) const override;

  private:
    int m_Id;
};

} // namespace PGM::OpenGL