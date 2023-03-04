#pragma once

#include "../../Core/Assert/Assert.h"
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

    constexpr void setAspectRatio(float aspect)
    {
        PGM_ASSERT(aspect > 0, "Invalid aspect ratio");
        m_AspectRatio = aspect;
        computeProjection();
    }

    constexpr void setOrtho(float size, float near, float far)
    {
        PGM_ASSERT(size > 0, "Null ortho size");
        PGM_ASSERT(near < far, "Invalid depth clip range");

        m_Type = CameraType::Orthographic;
        m_OrthoSize = size;
        m_Near = near;
        m_Far = far;

        computeProjection();
    }

    constexpr void setPerspective(float fov, float near, float far)
    {
        PGM_ASSERT(fov > 0 && fov <= 180, "Invalid fov value");
        PGM_ASSERT(near < far, "Invalid depth clip range");

        m_Type = CameraType::Perspective;
        m_Fov = fov;
        m_Near = near;
        m_Far = far;

        computeProjection();
    }

    constexpr CameraType type() const
    {
        return m_Type;
    }

    constexpr float aspectRatio() const
    {
        return m_AspectRatio;
    }

    constexpr float fov() const
    {
        return m_Fov;
    }

    constexpr float orthoSize() const
    {
        return m_OrthoSize;
    }

    constexpr float near() const
    {
        return m_Near;
    }

    constexpr float far() const
    {
        return m_Far;
    }

    constexpr const Mat4 &projection() const
    {
        return m_Projection;
    }

  private:
    CameraType m_Type;

    float m_AspectRatio = 1.0f;

    float m_Fov = 30.0f;

    float m_OrthoSize = 25.0f;

    float m_Near = 0.01f;
    float m_Far = 1000.0f;

    Mat4 m_Projection;

    constexpr void computeProjection()
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