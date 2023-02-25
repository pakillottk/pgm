#pragma once

#include "../../Commands.h"

namespace PGM::Renderer::API::Backend
{

struct OpenGlCommands : public Commands
{
    void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const override;
    void setViewport(const RectInt &viewportRect) const override;

    SharedRef<Buffers::GpuBuffer> createBuffer(bool dynamic, size_t size, const void *data = nullptr) const override;

    SharedRef<Buffers::VertexArray> createVertexArray(
        std::initializer_list<Buffers::VertexAttrib> attributes) const override;

    SharedRef<Buffers::VertexArray> createIndexedVertexArray(
        const Buffers::VertexAttrib &indexAttribute,
        std::initializer_list<Buffers::VertexAttrib> attributes) const override;
};

} // namespace PGM::Renderer::API::Backend