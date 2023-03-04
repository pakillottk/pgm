#pragma once

#include <array>
#include <glm/glm.hpp>

namespace PGM
{

using Color = glm::vec4;
using ByteColor = std::array<std::byte, 4>;

namespace Colors
{
    constexpr ByteColor toByteColor(const Color &c)
    {
        return ByteColor{static_cast<std::byte>(c.r * 255), static_cast<std::byte>(c.g * 255),
                         static_cast<std::byte>(c.b * 255), static_cast<std::byte>(c.a * 255)};
    }

    static constexpr Color Transparent{0.0f, 0.0f, 0.0f, 0.0f};

    static constexpr Color Black{0.0f, 0.0f, 0.0f, 1.0f};
    static constexpr Color White{1.0f, 1.0f, 1.0f, 1.0f};
    static constexpr Color Red{1.0f, 0.0f, 0.0f, 1.0f};
    static constexpr Color Green{0.0f, 1.0f, 0.0f, 1.0f};
    static constexpr Color Blue{0.0f, 0.0f, 1.0f, 1.0f};
    static constexpr Color Yellow{1.0f, 1.0f, 0.0f, 1.0f};
    static constexpr Color Cyan{0.0f, 1.0f, 1.0f, 1.0f};
    static constexpr Color Magenta{1.0f, 0.0f, 1.0f, 1.0f};

} // namespace Colors

} // namespace PGM