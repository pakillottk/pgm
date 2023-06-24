#include "VertexArrayImpl.h"

#include "../GlCheck.h"

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

namespace PGM::OpenGL
{

namespace detail
{
    inline int genVertexArray()
    {
        GLuint id = 0;
        glGenVertexArrays(1, &id);
        PGM_CHECK_GL();

        if (id == 0)
        {
            return PGM::NULL_VERTEX_ARRAY_ID;
        }

        PGM_CHECK_GL();
        return static_cast<int>(id);
    }

    static constexpr GLenum convertDataType(VertexAttribDataType dt)
    {
        switch (dt)
        {
        case PGM::VertexAttribDataType::Byte:
            return GL_BYTE;

        case PGM::VertexAttribDataType::UnsignedByte:
            return GL_UNSIGNED_BYTE;

        case PGM::VertexAttribDataType::Short:
            return GL_SHORT;

        case PGM::VertexAttribDataType::UnsignedShort:
            return GL_UNSIGNED_SHORT;

        case PGM::VertexAttribDataType::Int:
            return GL_INT;

        case PGM::VertexAttribDataType::Uint:
            return GL_UNSIGNED_INT;

        case PGM::VertexAttribDataType::Float:
            return GL_FLOAT;

        default:
            PGM_ASSERT(false, "Unknown vertex attrib data type");
            return 0;
        }
    }

    inline void attachVertexAttrib(int id, bool isIndex, const VertexAttrib &attrib)
    {
        if (isIndex)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attrib.buffer->id());
            PGM_CHECK_GL();
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, attrib.buffer->id());

            glVertexAttribPointer(attrib.layout.location, static_cast<GLint>(attrib.layout.size),
                                  convertDataType(attrib.layout.type), attrib.layout.normalize ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(attrib.layout.stride),
                                  reinterpret_cast<void *>(attrib.layout.offset));
            PGM_CHECK_GL();

            glEnableVertexArrayAttrib(id, attrib.layout.location);
            PGM_CHECK_GL();
        }
    }

} // namespace detail

int VertexArrayImpl::genVertexArray(std::initializer_list<VertexAttrib> buffers)
{
    int id = detail::genVertexArray();
    if (id == PGM::NULL_VERTEX_ARRAY_ID)
    {
        return id;
    }

    bindVertexArray(id);
    std::for_each(buffers.begin(), buffers.end(),
                  [id](const auto &attrib) { detail::attachVertexAttrib(id, false, attrib); });
    unbindVertexArray();

    return id;
}

int VertexArrayImpl::genVertexArray(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers)
{
    int id = detail::genVertexArray();
    if (id == PGM::NULL_VERTEX_ARRAY_ID)
    {
        return id;
    }

    bindVertexArray(id);
    detail::attachVertexAttrib(id, true, indexBuffer);
    std::for_each(buffers.begin(), buffers.end(),
                  [id](const auto &attrib) { detail::attachVertexAttrib(id, false, attrib); });
    m_Attribs.push_back(indexBuffer);
    unbindVertexArray();

    return id;
}

void VertexArrayImpl::bindVertexArray(int id) const
{
    glBindVertexArray(id);
    PGM_CHECK_GL();
}

void VertexArrayImpl::unbindVertexArray() const
{
    glBindVertexArray(0);
    PGM_CHECK_GL();
}

} // namespace PGM::OpenGL