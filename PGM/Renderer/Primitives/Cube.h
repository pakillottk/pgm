#pragma once

#include "../Mesh/Mesh.h"

namespace PGM::Primitives
{

inline Mesh cube(float size = 1.0f)
{
    const float halfSize = size / 2;

    Mesh mesh;

    // NOTE(pgm) We duplicate the geometry
    // to avoid normal lerping issues

    // clang-format off
    mesh.vertices = {
        // Front
        Vec3{-halfSize, -halfSize, -halfSize},
        Vec3{halfSize, -halfSize, -halfSize},
        Vec3{halfSize, halfSize, -halfSize},
        Vec3{-halfSize, halfSize, -halfSize},

        // Back
        Vec3{-halfSize, -halfSize, halfSize},
        Vec3{halfSize, -halfSize, halfSize},
        Vec3{halfSize, halfSize, halfSize},
        Vec3{-halfSize, halfSize, halfSize},

        // Top
        Vec3{-halfSize, halfSize, -halfSize},
        Vec3{halfSize, halfSize, -halfSize},
        Vec3{halfSize, halfSize, halfSize},
        Vec3{-halfSize, halfSize, halfSize},

        // Bottom
        Vec3{-halfSize, -halfSize, -halfSize},
        Vec3{halfSize, -halfSize, -halfSize},
        Vec3{halfSize, -halfSize, halfSize},
        Vec3{-halfSize, -halfSize, halfSize},

        // Left
        Vec3{-halfSize, -halfSize, -halfSize},
        Vec3{-halfSize, -halfSize, halfSize},
        Vec3{-halfSize, halfSize, halfSize},
        Vec3{-halfSize, halfSize, -halfSize},

        // Right
        Vec3{halfSize, -halfSize, -halfSize},
        Vec3{halfSize, -halfSize, halfSize},
        Vec3{halfSize, halfSize, halfSize},
        Vec3{halfSize, halfSize, -halfSize},
    };
    mesh.normals = {
        // Front
        Vec3{0.0, -1.0f, 0.0f},
        Vec3{0.0, -1.0f, 0.0f},
        Vec3{0.0, -1.0f, 0.0f},
        Vec3{0.0, -1.0f, 0.0f},

        // Back
        Vec3{0.0,1.0f, 0.0f},
        Vec3{0.0,1.0f, 0.0f},
        Vec3{0.0,1.0f, 0.0f},
        Vec3{0.0,1.0f, 0.0f},

        // Top
        Vec3{0.0, 0.0f, 1.0f},
        Vec3{0.0, 0.0f, 1.0f},
        Vec3{0.0, 0.0f, 1.0f},
        Vec3{0.0, 0.0f, 1.0f},

        // Bottom
        Vec3{0.0, 0.0f, -1.0f},
        Vec3{0.0, 0.0f, -1.0f},
        Vec3{0.0, 0.0f, -1.0f},
        Vec3{0.0, 0.0f, -1.0f},

        // Left
        Vec3{-1.0, 0.0f, 0.0f},
        Vec3{-1.0, 0.0f, 0.0f},
        Vec3{-1.0, 0.0f, 0.0f},
        Vec3{-1.0, 0.0f, 0.0f},

        // Right
        Vec3{1.0, 0.0f, 0.0f},
        Vec3{1.0, 0.0f, 0.0f},
        Vec3{1.0, 0.0f, 0.0f},
        Vec3{1.0, 0.0f, 0.0f}
    };
    
    // TODO UVs?

    mesh.faces = 
    {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Back
        4, 5, 6,
        6, 7, 4,

        // Top
        8, 9, 10,
        10, 11, 8,

        // Bottom
        12, 13, 14,
        14, 15, 12,

        // Left
        16, 17, 18,
        18, 19, 16,

        // Right
        20, 21, 22,
        22, 23, 20
    };

    // clang-format on

    return mesh;
}

} // namespace PGM::Primitives