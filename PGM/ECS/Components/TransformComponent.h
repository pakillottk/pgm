#pragma once

#include "../../Core/Matrix/Matrix.h"
#include "../../Core/Vec/Vec.h"

namespace PGM::Components
{

struct TransformComponent
{
    Vec3 position = {};
    Vec3 rotation = {};
    Vec3 scale = {1.0f, 1.0f, 1.0f};

    inline Mat4 toMatrix() const
    {
        return glm::translate(position) * glm::toMat4(glm::quat(rotation)) * glm::scale(scale);
    }

    operator Mat4() const
    {
        return toMatrix();
    }
};

} // namespace PGM::Components
