#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace PGM
{

using Mat4 = glm::mat4;

static constexpr Mat4 Identity4x4 = Mat4();

} // namespace PGM