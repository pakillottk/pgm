#pragma once

#include "../../RendererCommands.h"

namespace PGM::OpenGL
{

struct OpenGlCommands : public RendererCommands
{
    void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const override;
    void setViewport(const RectInt &viewportRect) const override;
    void setClipRegion(const RectInt &clipRect) const override;

    void depthTest(bool enable) const override;
    void blending(bool enable) const override;

    SharedRef<GpuBuffer> createBuffer(bool dynamic, size_t size, const void *data = nullptr) const override;

    SharedRef<VertexArray> createVertexArray(std::initializer_list<VertexAttrib> attributes) const override;

    SharedRef<VertexArray> createIndexedVertexArray(const VertexAttrib &indexAttribute,
                                                    std::initializer_list<VertexAttrib> attributes) const override;

    SharedRef<Shader> createShader(std::string_view vertexSource,
                                   std::string_view fragmentSource) const override;

    SharedRef<Texture2d> createTexture2d(PixelType pixelType, unsigned channelCount, unsigned w, unsigned h,
                                         const void *data = nullptr) const override;

    void drawIndexed(PrimitiveType primitive, unsigned elements, VertexAttribDataType indexType,
                     size_t offset) const override;

    void draw(PrimitiveType primitive, unsigned vertexCount, size_t offset) const override;
};

} // namespace PGM::OpenGL