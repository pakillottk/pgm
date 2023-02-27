#pragma once

#include "../../Commands.h"

namespace PGM::Renderer::API::Backend
{

struct OpenGlCommands : public Commands
{
    void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const override;
    void setViewport(const RectInt &viewportRect) const override;
    void setClipRegion(const RectInt &clipRect) const override;

    void depthTest(bool enable) const override;
    void blending(bool enable) const override;

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

    void drawIndexed(PrimitiveType primitive, unsigned elements, Buffers::VertexAttribDataType indexType,
                     size_t offset) const override;

    void draw(PrimitiveType primitive, unsigned vertexCount, size_t offset) const override;
};

} // namespace PGM::Renderer::API::Backend