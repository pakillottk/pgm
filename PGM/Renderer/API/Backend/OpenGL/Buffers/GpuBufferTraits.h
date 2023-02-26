#pragma once

#include "../../../Buffers/GpuBuffer.h"
#include "../GlCheck.h"

#ifdef _WIN32
// On windows this header is required for certain types and macros...
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <gl/GL.h>

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace PGM::Renderer::API::Backend::OpenGL::Buffers
{

namespace detail
{
    inline int genGlBuffer()
    {
        GLuint id = 0;
        glGenBuffers(1, &id);
        PGM_CHECK_GL();
        if (id == 0)
        {
            return PGM::Renderer::API::Buffers::NULL_BUFFER_ID;
        }
        glBindBuffer(GL_ARRAY_BUFFER, id);
        PGM_CHECK_GL();
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return static_cast<int>(id);
    }

    inline void destroyGlBuffer(int id)
    {
        GLuint glId{static_cast<GLuint>(id)};
        glDeleteBuffers(1, &glId);
        PGM_CHECK_GL();
    }
} // namespace detail

struct OpenGlStaticBufferTraits final
{
    inline bool dynamic() const
    {
        return false;
    }

    inline int genBuffer()
    {
        return detail::genGlBuffer();
    }

    inline void destroy(int id)
    {
        detail::destroyGlBuffer(id);
    }

    inline void allocate(int id, size_t size, const void *data) const
    {
        glNamedBufferData(id, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
        PGM_CHECK_GL();
    }

    inline void write(int id, size_t offset, size_t size, const void *data) const
    {
        glNamedBufferSubData(id, static_cast<GLintptr>(offset), static_cast<GLintptr>(size), data);
        PGM_CHECK_GL();
    }

    inline void commit([[maybe_unused]] int id)
    {
    }
};

struct OpenGlDynamicBufferTraits final
{
  private:
    mutable std::vector<std::byte> m_Data;

  public:
    inline bool dynamic() const
    {
        return true;
    }

    inline int genBuffer()
    {
        return detail::genGlBuffer();
    }

    inline void destroy(int id)
    {
        detail::destroyGlBuffer(id);
        // NOTE(pgm) This ensures that memory is released
        m_Data = std::vector<std::byte>{};
    }

    inline void allocate([[maybe_unused]] int id, size_t size, const void *data) const
    {
        m_Data.resize(size);
        if (data != nullptr)
        {
            memcpy(&m_Data[0], data, size);
        }
    }

    inline void write([[maybe_unused]] int id, size_t offset, size_t size, const void *data) const
    {
        if ((offset + size) > m_Data.size())
        {
            m_Data.resize(offset + size);
        }
        memcpy(&m_Data[0] + offset, data, size);
    }

    inline void commit(int id)
    {
        glNamedBufferData(id, static_cast<GLsizeiptr>(m_Data.size()), m_Data.data(), GL_STREAM_DRAW);
        PGM_CHECK_GL();
    }
};

} // namespace PGM::Renderer::API::Backend::OpenGL::Buffers