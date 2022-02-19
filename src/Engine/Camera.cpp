#include <Engine/Camera.hpp>

using namespace glm;

Camera::Camera() : m_transform(Transform()), m_fov(60), m_nearPlane(0.03f), m_farPlane(100.0f)
{

}

mat4 Camera::GetProjectionMatrix(GLFWwindow* window) const
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

	return perspective(radians(m_fov), (float)width / height, m_nearPlane, m_farPlane);
}