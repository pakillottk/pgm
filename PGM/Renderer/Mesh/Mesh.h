#pragma once

#include "../../Core/Color/Color.h"
#include "../../Core/Vec/Vec.h"

#include <vector>

namespace PGM
{

class Mesh
{
  public:
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> uv;
    std::vector<ByteColor> vertexColor;

    std::vector<unsigned> faces;
};

} // namespace PGM