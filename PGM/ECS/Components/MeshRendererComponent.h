#pragma once

#include "../../Renderer/Mesh/Mesh.h"

namespace PGM::Components
{

struct MeshRendererComponent
{
    Mesh mesh;
    // TODO material...

    void *rendererData;
};

} // namespace PGM::Components