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

    SharedRef<Shaders::Shader> createShader(const std::string_view &vertexSource,
                                            const std::string_view &fragmentSource) const override;

    SharedRef<Textures::Texture2d> createTexture2d(Textures::PixelType pixelType, unsigned channelCount, unsigned w,
                                                   unsigned h, const void *data = nullptr) const override;
};

} // namespace PGM::Renderer::API::Backend