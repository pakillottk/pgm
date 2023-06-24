#pragma once

#include "../../Core/Assert/Assert.h"
#include "../API/Buffers/VertexArray.h"
#include "../RenderContext.h"

#include <algorithm>

namespace PGM
{

using BatchLayout = std::vector<VertexAttribLayout>;

class StaticBatch
{
  public:
    template <typename TVertex>
    inline StaticBatch(const RenderContext &ctx, PrimitiveType primitive, const BatchLayout &batchLayout,
                       size_t batchCapacity, const TVertex *data = nullptr)
        : primitiveType{primitive}, m_Capacity{batchCapacity}, m_Size{data != nullptr ? batchCapacity : 0}
    {
        PGM_ASSERT(m_Capacity > 0);
        PGM_ASSERT(!batchLayout.empty());

        const size_t bufferSize = m_Capacity;
        m_Buffer = ctx->createBuffer(false, bufferSize, reinterpret_cast<const void *>(data));
        m_VertexCount = data != nullptr ? static_cast<unsigned>(m_Size / sizeof(TVertex)) : 0;

        std::vector<VertexAttrib> attribs{};
        std::transform(batchLayout.begin(), batchLayout.end(), std::back_inserter(attribs),
                       [this](const VertexAttribLayout &attrib) -> VertexAttrib {
                           return VertexAttrib{m_Buffer, attrib};
                       });
        m_VertexArray = ctx->createVertexArray(
            std::initializer_list<VertexAttrib>(attribs.data(), attribs.data() + attribs.size()));
    }

    StaticBatch(const StaticBatch &) = default;
    StaticBatch &operator=(const StaticBatch &) = default;

    constexpr size_t capacity() const
    {
        return m_Capacity;
    }

    constexpr size_t size() const
    {
        return m_Size;
    }

    constexpr unsigned vertexCount() const
    {
        return m_VertexCount;
    }

    constexpr const PGM::SharedRef<VertexArray> &vertexArray() const
    {
        return m_VertexArray;
    }

    template <typename TVertex> inline void pushVertices(const TVertex *data, size_t size)
    {
        PGM_ASSERT(data != nullptr);
        PGM_ASSERT(size > 0);
        PGM_ASSERT((m_Size + size) <= m_Capacity);
        m_Buffer->write(m_Size, size, reinterpret_cast<const void *>(data));
        m_Size += size;
        m_VertexCount += static_cast<unsigned>(size / sizeof(TVertex));
    }

    PrimitiveType primitiveType;

  protected:
    size_t m_Capacity;
    size_t m_Size;
    unsigned m_VertexCount;

    PGM::SharedRef<GpuBuffer> m_Buffer;
    PGM::SharedRef<VertexArray> m_VertexArray;
};

} // namespace PGM