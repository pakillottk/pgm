#pragma once

#include "../../../Buffers/VertexArray.h"

namespace PGM::Renderer::API::Backend::OpenGL::Buffers
{

class VertexArrayImpl final : public PGM::Renderer::API::Buffers::VertexArray
{
  public:
    inline VertexArrayImpl(std::initializer_list<PGM::Renderer::API::Buffers::VertexAttrib> buffers)
        : PGM::Renderer::API::Buffers::VertexArray(buffers)
    {
        m_Id = genVertexArray(buffers);
    }

    inline VertexArrayImpl(const PGM::Renderer::API::Buffers::VertexAttrib &indexBuffer,
                           std::initializer_list<PGM::Renderer::API::Buffers::VertexAttrib> buffers)
        : PGM::Renderer::API::Buffers::VertexArray(indexBuffer, buffers)
    {
        m_Id = genVertexArray(indexBuffer, buffers);
    }

  protected:
    int genVertexArray(std::initializer_list<PGM::Renderer::API::Buffers::VertexAttrib> buffers) const override;

    int genVertexArray(const PGM::Renderer::API::Buffers::VertexAttrib &indexBuffer,
                       std::initializer_list<PGM::Renderer::API::Buffers::VertexAttrib> buffers) const override;

    void bindVertexArray(int id) const override;

    void unbindVertexArray() const override;
};

} // namespace PGM::Renderer::API::Backend::OpenGL::Buffers