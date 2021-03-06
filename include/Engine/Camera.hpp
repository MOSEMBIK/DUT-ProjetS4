#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Engine/Transform.hpp>
#include <Engine/Event/EventReceiver.hpp>

class Camera : public IEventReceiver
{
private:
    Transform m_transform;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

    bool m_enableRotation;

public:
    Camera();

    virtual ~Camera() = default;

    /**
     * @brief Get the Transform of the Camera
     * 
     * @return Transform& 
     */
    inline Transform& getTransform() { return m_transform; }

    /**
     * @brief Get the View Matrix of the Camera
     * 
     * @return glm::mat4 
     */
    glm::mat4 getViewMatrix() const;

    /**
     * @brief Get the Projection Matrix of the Camera
     * 
     * @param window 
     * @return glm::mat4 
     */
    glm::mat4 getProjectionMatrix(GLFWwindow* window) const;

    /**
     * @brief Get current FOV (field of view) (in degree)
     * 
     * @return float 
     */
    inline float getFOV() const { return m_fov; }

    /**
     * @brief Set current FOV (field of view) (in degree)
     * 
     * @param fov 
     */
    inline void setFOV(float fov) { m_fov = fov; }

    /**
     * @brief Get the Near Plane distance (minimum distance to render)
     * 
     * @return float 
     */
    inline float getNearPlane() const { return m_nearPlane; }

    /**
     * @brief Set the Near Plane distance (minimum distance to render)
     * 
     * @param nearPlane 
     */
    inline void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

    /**
     * @brief Get the Far Plane distance (maximum distance to render)
     * 
     * @return float 
     */
    inline float getFarPlane() const { return m_farPlane; }

    /**
     * @brief Set the Far Plane distance (maximum distance to render)
     * 
     * @return float 
     */
    inline void setFarPlane(float farPlane) { m_farPlane = farPlane; }

    virtual void onEvent(Event& e);

    bool onMouseMoved(MouseMovedEvent& e);
    bool onMouseButtonPressed(MouseButtonPressedEvent& e);
    bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
    bool onAppUpdate(AppUpdateEvent& e);
};
