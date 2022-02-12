#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Engine/Transform.hpp>

class Camera
{
private:
    Camera();

    Transform m_transform;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

    // Singleton
    static Camera* m_instance;

public:
    // Suppression du clonage et de l'opérateur =
    Camera(Camera&) = delete;
    void operator= (const Camera&) = delete;

    /**
     * @brief Get the Singleton instance of Camera
     * 
     * @return Camera* 
     */
    static Camera* GetInstance();

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
    inline glm::mat4 GetViewMatrix() { return m_transform.GetTRSMatrix(); }

    /**
     * @brief Get the Projection Matrix of the Camera
     * 
     * @param window 
     * @return glm::mat4 
     */
    glm::mat4 GetProjectionMatrix(GLFWwindow* window);
};
