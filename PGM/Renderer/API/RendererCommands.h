#pragma once

#include "Buffers/GpuBuffer.h"
#include "Buffers/VertexArray.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2d.h"

#include "../../Core/Color/Color.h"
#include "../../Core/Rect/Rect.h"
#include "../../Core/Ref/Ref.h"

namespace PGM
{

enum ClearBufferBits
{
    bColor = 0b001,
    bDepth = 0b010,
    bStencil = 0b100
};

enum PrimitiveType
{
    Triangles
};

using ClearBufferMask = int;

struct RendererCommands
{
    virtual void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const = 0;
    virtual void setViewport(const RectInt &viewportRect) const = 0;
    virtual void setClipRegion(const RectInt &clipRect) const = 0;

    virtual void depthTest(bool enable) const = 0;
    virtual void blending(bool enable) const = 0;

    virtual SharedRef<GpuBuffer> createBuffer(bool dynamic, size_t size, const void *data = nullptr) const = 0;

    virtual SharedRef<VertexArray> createVertexArray(std::initializer_list<VertexAttrib> attributes) const = 0;
    virtual SharedRef<VertexArray> createIndexedVertexArray(const VertexAttrib &indexAttribute,
                                                            std::initializer_list<VertexAttrib> attributes) const = 0;

    // TODO(pgm) This is not crossplatform but for now will suffice...
    virtual SharedRef<Shader> createShader(std::string_view vertexSource,
                                           std::string_view fragmentSource) const = 0;

    virtual SharedRef<Texture2d> createTexture2d(PixelType pixelType, unsigned channelCount, unsigned w, unsigned h,
                                                 const void *data = nullptr) const = 0;

    virtual void drawIndexed(PrimitiveType primitive, unsigned elements, VertexAttribDataType indexType,
                             size_t offset) const = 0;

    virtual void draw(PrimitiveType primitive, unsigned vertexCount, size_t offset) const = 0;
};

} // namespace PGM