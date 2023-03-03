#pragma once

#include "../Core/Ref/Ref.h"

#include "../Renderer/API/Buffers/VertexArray.h"
#include "../Renderer/API/Shaders/Shader.h"
#include "../Renderer/API/Textures/Texture2d.h"
#include "../Renderer/Renderer.h"

struct ImDrawData;

namespace PGM::GUI
{

struct RenderData
{
    const Renderer *context;

    SharedRef<Shader> shader;

    SharedRef<Texture2d> fontAtlas;

    SharedRef<GpuBuffer> vertices;
    SharedRef<GpuBuffer> indices;

    SharedRef<VertexArray> vao;

    int projMatLoc;
    int textSamplerLoc;
};

RenderData initializeRenderData(const Renderer &ctx);
void destroyRenderData(RenderData &renderData);

void setupImGuiStyle();
void renderImGui(ImDrawData *draw_data, RenderData &renderData);

} // namespace PGM::GUI