#pragma once

#include "Buffers/GpuBuffer.h"

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
};

} // namespace PGM::Renderer::API