#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Engine/Transform.hpp>

class Camera
{
private:
    Transform m_transform;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

public:
    Camera();

    /**
     * @brief Get the Transform of the Camera
     * 
     * @return Transform& 
     */
    inline Transform& GetTransform() { return m_transform; }

    /**
     * @brief Get the View Matrix of the Camera
     * 
     * @return glm::mat4 
     */
    inline glm::mat4 GetViewMatrix() const { return m_transform.GetTRSMatrix(); }

    /**
     * @brief Get the Projection Matrix of the Camera
     * 
     * @param window 
     * @return glm::mat4 
     */
    glm::mat4 GetProjectionMatrix(GLFWwindow* window) const;

    /**
     * @brief Get current FOV (field of view) (in degree)
     * 
     * @return float 
     */
    inline float GetFOV() const { return m_fov; }

    /**
     * @brief Set current FOV (field of view) (in degree)
     * 
     * @param fov 
     */
    inline void SetFOV(float fov) { m_fov = fov; }

    /**
     * @brief Get the Near Plane distance (minimum distance to render)
     * 
     * @return float 
     */
    inline float GetNearPlane() const { return m_nearPlane; }

    /**
     * @brief Set the Near Plane distance (minimum distance to render)
     * 
     * @param nearPlane 
     */
    inline void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

    /**
     * @brief Get the Far Plane distance (maximum distance to render)
     * 
     * @return float 
     */
    inline float GetFarPlane() const { return m_farPlane; }

    /**
     * @brief Set the Far Plane distance (maximum distance to render)
     * 
     * @return float 
     */
    inline void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
};
