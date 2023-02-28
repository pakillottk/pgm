#pragma once

#include <PGM/Core/Ref/Ref.h>

#include <PGM/Renderer/API/Buffers/VertexArray.h>
#include <PGM/Renderer/API/Shaders/Shader.h>
#include <PGM/Renderer/API/Textures/Texture2d.h>
#include <PGM/Renderer/RenderContext.h>

struct ImDrawData;

namespace PGM::GUI
{

struct RenderData
{
    const Renderer::RenderContext *context;

    SharedRef<Renderer::API::Shaders::Shader> shader;

    SharedRef<Renderer::API::Textures::Texture2d> fontAtlas;

    SharedRef<Renderer::API::Buffers::GpuBuffer> vertices;
    SharedRef<Renderer::API::Buffers::GpuBuffer> indices;

    SharedRef<Renderer::API::Buffers::VertexArray> vao;

    int projMatLoc;
    int textSamplerLoc;
};

RenderData initializeRenderData(const Renderer::RenderContext &ctx);
void destroyRenderData(RenderData &renderData);

void setupImGuiStyle();
void renderImGui(ImDrawData *draw_data, RenderData &renderData);

} // namespace PGM::GUI