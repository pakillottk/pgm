#pragma once

#include "../../Core/Matrix/Matrix.h"

namespace PGM
{

enum CameraType
{
    Orthographic,
    Perspective
};

class Camera
{
  public:
    inline Camera(CameraType type = PGM::Perspective) : m_Type{type}
    {
        computeProjection();
    }

    inline void setAspectRatio(float aspect)
    {
        PGM_ASSERT(aspect > 0, "Invalid aspect ratio");
        m_AspectRatio = aspect;
        computeProjection();
    }

    constexpr const Mat4 &projection() const
    {
        return m_Projection;
    }

  private:
    CameraType m_Type;

    float m_Fov = 60.0f;

    float m_AspectRatio = 1.0f;

    float m_OrthoSize = 10.0f;
    float m_Near = 0.01f;
    float m_Far = 1000.0f;

    Mat4 m_Projection;

    inline void computeProjection()
    {
        if (m_Type == PGM::Perspective)
        {
            m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
        }
        else
        {
            float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
            float orthoRight = m_OrthoSize * m_AspectRatio * 0.5f;
            float orthoBottom = -m_OrthoSize * 0.5f;
            float orthoTop = m_OrthoSize * 0.5f;

            m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Near, m_Far);
        }
    }
};

} // namespace PGM