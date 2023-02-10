#pragma once

#include <glm/glm.hpp>

namespace PGM
{

using Color = glm::vec4;

namespace Colors
{
    static constexpr Color Transparent{0.0f, 0.0f, 0.0f, 0.0f};

    static constexpr Color Black{0.0f, 0.0f, 0.0f, 1.0f};
    static constexpr Color Red{1.0f, 0.0f, 0.0f, 1.0f};

} // namespace Colors

} // namespace PGM