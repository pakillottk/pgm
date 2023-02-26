#pragma once

#include "GpuBuffer.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Ref/Ref.h>

#include <algorithm>
#include <initializer_list>
#include <vector>

namespace PGM::Renderer::API::Buffers
{

enum AttribLocations : unsigned
{
    // Coordinates XYZ
    Position = 0,

    // Normal mapping
    Normal = 1,
    Tangent = 2,
    Binormal = 3,

    // Texcoords
    UV0 = 4
};

enum VertexAttribDataType
{
    Byte,
    UnsignedByte,
    Short,
    UnsignedShort,
    Int,
    Uint,
    Float
};

struct VertexAttrib
{
    SharedRef<GpuBuffer> buffer;
    unsigned location;
    VertexAttribDataType type;
    unsigned size;
    size_t stride;
    size_t offset;
    bool normalize = false;
};

static constexpr int NULL_VERTEX_ARRAY_ID = -1;

class VertexArray
{
  public:
    VertexArray(std::initializer_list<VertexAttrib> buffers);

    VertexArray(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers);

    ~VertexArray();

    void destroy();

    inline operator bool() const
    {
        return m_Id == NULL_VERTEX_ARRAY_ID;
    }

    inline int id() const
    {
        return m_Id;
    }

    inline bool indexed() const
    {
        return m_Indexed;
    }

    void bind() const;

    void unbind() const;

  protected:
    int m_Id;
    bool m_Indexed;
    std::vector<VertexAttrib> m_Attribs;

    virtual int genVertexArray(std::initializer_list<VertexAttrib> buffers) = 0;
    virtual int genVertexArray(const VertexAttrib &indexBuffer, std::initializer_list<VertexAttrib> buffers) = 0;
    virtual void bindVertexArray(int id) const = 0;
    virtual void unbindVertexArray() const = 0;

  private:
    static inline void commitVertexAttrib(const VertexAttrib &attrib)
    {
        attrib.buffer->commit();
    }

    static inline void destroyVertexAttrib(const VertexAttrib &attrib)
    {
        attrib.buffer->destroy();
    }
};

} // namespace PGM::Renderer::API::Buffers