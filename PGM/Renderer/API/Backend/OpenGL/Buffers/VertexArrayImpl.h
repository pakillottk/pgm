#pragma once

#include "../../../Buffers/VertexArray.h"

namespace PGM::OpenGL
{

class VertexArrayImpl final : public VertexArray
{
  public:
    inline VertexArrayImpl(std::initializer_list<VertexAttrib> buffers) : VertexArray(buffers)
    {
        m_Id = genVertexArray(buffers);
    }

    inline VertexArrayImpl(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers)
        : VertexArray(indexBuffer, buffers)
    {
        m_Id = genVertexArray(indexBuffer, buffers);
    }

  protected:
    int genVertexArray(std::initializer_list<VertexAttrib> buffers) override;

    int genVertexArray(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers) override;

    void bindVertexArray(int id) const override;

    void unbindVertexArray() const override;
};

} // namespace PGM::OpenGL