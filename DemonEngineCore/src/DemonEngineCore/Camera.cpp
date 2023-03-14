#include "DemonEngineCore/Camera.hpp"
#include <glm/trigonometric.hpp>

namespace DemonEngine {
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const ProjectionMode projectionMode)
        : mPosition(position)
        , mRotation(rotation)
        , mProjectionMode(projectionMode)
    {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    void Camera::updateViewMatrix()
    {
        float rotate_in_radians_x = glm::radians(-mRotation.x);
        glm::mat4 rotate_matrix_x(1, 0, 0, 0,
            0, cos(rotate_in_radians_x), sin(rotate_in_radians_x), 0,
            0, -sin(rotate_in_radians_x), cos(rotate_in_radians_x), 0,
            0, 0, 0, 1);

        float rotate_in_radians_y = glm::radians(-mRotation.y);
        glm::mat4 rotate_matrix_y(cos(rotate_in_radians_y), 0, -sin(rotate_in_radians_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_radians_y), 0, cos(rotate_in_radians_y), 0,
            0, 0, 0, 1);

        float rotate_in_radians_z = glm::radians(-mRotation.z);
        glm::mat4 rotate_matrix(cos(rotate_in_radians_z), sin(rotate_in_radians_z), 0, 0,
            -sin(rotate_in_radians_z), cos(rotate_in_radians_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -mPosition[0], -mPosition[1], -mPosition[2], 1);

        mViewMatrix = rotate_matrix_y * rotate_matrix_x * translate_matrix;
    }

    void Camera::updateProjectionMatrix()
    {
        if (mProjectionMode == ProjectionMode::Perspective)
        {
            float r = 0.1f;
            float t = 0.1f;
            float f = 10;
            float n = 0.1f;
            mProjectionMatrix = glm::mat4(n / r, 0, 0, 0,
                0, n / t, 0, 0,
                0, 0, (-f - n) / (f - n), -1,
                0, 0, -2 * f * n / (f - n), 0);
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            mProjectionMatrix = glm::mat4(1 / r, 0, 0, 0,
                0, 1 / t, 0, 0,
                0, 0, -2 / (f - n), 0,
                0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::setPosition(const glm::vec3& position)
    {
        mPosition = position;
        updateViewMatrix();
    }

    void Camera::setRotation(const glm::vec3& rotation)
    {
        mRotation = rotation;
        updateViewMatrix();
    }

    void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
    {
        mPosition = position;
        mRotation = rotation;
        updateViewMatrix();
    }

    void Camera::setProjectionMode(const ProjectionMode projectionMode)
    {
        mProjectionMode = projectionMode;
        updateProjectionMatrix();
    }
}