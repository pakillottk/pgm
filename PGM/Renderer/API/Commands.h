#pragma once

#include "Buffers/GpuBuffer.h"
#include "Buffers/VertexArray.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2d.h"

#include <PGM/Core/Color/Color.h>
#include <PGM/Core/Rect/Rect.h>
#include <PGM/Core/Ref/Ref.h>

namespace PGM::Renderer::API
{

enum ClearBufferBits
{
    bColor = 0b001,
    bDepth = 0b010,
    bStencil = 0b100
};

using ClearBufferMask = int;

struct Commands
{
    virtual void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const = 0;
    virtual void setViewport(const RectInt &viewportRect) const = 0;

    virtual SharedRef<Buffers::GpuBuffer> createBuffer(bool dynamic, size_t size, const void *data = nullptr) const = 0;

    virtual SharedRef<Buffers::VertexArray> createVertexArray(
        std::initializer_list<Buffers::VertexAttrib> attributes) const = 0;
    virtual SharedRef<Buffers::VertexArray> createIndexedVertexArray(
        const Buffers::VertexAttrib &indexAttribute, std::initializer_list<Buffers::VertexAttrib> attributes) const = 0;

    // TODO(pgm) This is not crossplatform but for now will suffice...
    virtual SharedRef<Shaders::Shader> createShader(const std::string_view &vertexSource,
                                                    const std::string_view &fragmentSource) const = 0;

    virtual SharedRef<Textures::Texture2d> createTexture2d(Textures::PixelType pixelType, unsigned channelCount,
                                                           unsigned w, unsigned h,
                                                           const void *data = nullptr) const = 0;
};

} // namespace PGM::Renderer::API