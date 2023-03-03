#include "VertexArray.h"

namespace PGM
{

VertexArray::VertexArray(std::initializer_list<VertexAttrib> buffers) : m_Attribs{buffers}, m_Indexed{false}
{
}

VertexArray::VertexArray(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers)
    : m_Attribs{buffers}, m_Indexed{true}
{
}

VertexArray::~VertexArray()
{
    destroy();
}

void VertexArray::destroy()
{
    if (m_Id == NULL_VERTEX_ARRAY_ID)
    {
        return;
    }

    std::for_each(m_Attribs.begin(), m_Attribs.end(), &VertexArray::destroyVertexAttrib);
    m_Attribs.clear();
    m_Id = NULL_VERTEX_ARRAY_ID;
}

void VertexArray::bind() const
{
    PGM_ASSERT(m_Id != NULL_VERTEX_ARRAY_ID, "Invalid vertex array");
    std::for_each(m_Attribs.begin(), m_Attribs.end(), &VertexArray::commitVertexAttrib);
    bindVertexArray(m_Id);
}

void VertexArray::unbind() const
{
    unbindVertexArray();
}

} // namespace PGM