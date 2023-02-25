#pragma once

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Ref/Ref.h>

namespace PGM::Renderer::API::Buffers
{

static constexpr int NULL_BUFFER_ID = -1;

struct BufferTraitsInterface
{
    virtual ~BufferTraitsInterface() = default;

    virtual bool dynamic() const = 0;

    virtual int genBuffer() = 0;
    virtual void destroy(int id) = 0;
    virtual void allocate(int id, size_t size, const void *data) = 0;
    virtual void write(int id, size_t offset, size_t size, const void *data) = 0;
    virtual void commit(int id) = 0;
};

class GpuBuffer
{
  public:
    template <typename BufferTraits>
    inline GpuBuffer(BufferTraits traits, size_t size, const void *data = nullptr)
        : m_Buffer{make_ref<Buffer<BufferTraits>>(std::move(traits))}
    {
        m_Id = m_Buffer->genBuffer();
        allocate(size, data);
    }

    inline ~GpuBuffer()
    {
        if (m_Id != NULL_BUFFER_ID)
        {
            destroy();
        }
    }

    inline operator bool() const
    {
        return m_Id != NULL_BUFFER_ID;
    }

    inline int id() const
    {
        PGM_ASSERT(m_Id != NULL_BUFFER_ID, "Invalid buffer id");
        return m_Id;
    }

    inline bool dynamic() const
    {
        return m_Buffer->dynamic();
    }

    inline void destroy()
    {
        if (m_Id == NULL_BUFFER_ID)
        {
            return;
        }

        m_Buffer->destroy(m_Id);
        m_Id = NULL_BUFFER_ID;
    }

    inline void allocate(size_t size, const void *data = nullptr)
    {
        PGM_ASSERT(m_Id != NULL_BUFFER_ID, "Invalid buffer id");
        m_Buffer->allocate(m_Id, size, data);
        m_Size = size;
    }

    inline void write(size_t offset, size_t size, const void *data)
    {
        PGM_ASSERT(m_Id != NULL_BUFFER_ID, "Invalid buffer id");
        PGM_ASSERT(dynamic() || (offset + size) <= m_Size, "Out of buffer bounds");
        m_Buffer->write(m_Id, offset, size, data);
    }

    template <typename T> inline void write(size_t offset, const T &data)
    {
        PGM_ASSERT(m_Id != NULL_BUFFER_ID, "Invalid buffer id");
        write(offset, sizeof(T), &data);
    }

    inline void commit()
    {
        PGM_ASSERT(m_Id != NULL_BUFFER_ID, "Invalid buffer id");
        m_Buffer->commit(m_Id);
    }

  private:
    template <typename T> struct Buffer final : BufferTraitsInterface
    {
        T m_Buffer;

        Buffer(T buff) : m_Buffer(std::move(buff))
        {
        }

        inline bool dynamic() const override
        {
            return m_Buffer.dynamic();
        }

        inline int genBuffer() override
        {
            return m_Buffer.genBuffer();
        }

        virtual void destroy(int id) override
        {
            m_Buffer.destroy(id);
        }

        inline void allocate(int id, size_t size, const void *data) override
        {
            m_Buffer.allocate(id, size, data);
        }

        inline void write(int id, size_t offset, size_t size, const void *data) override
        {
            m_Buffer.write(id, offset, size, data);
        }

        inline void commit(int id) override
        {
            m_Buffer.commit(id);
        }
    };

    int m_Id;
    Ref<BufferTraitsInterface> m_Buffer;
    size_t m_Size;
};

} // namespace PGM::Renderer::API::Buffers