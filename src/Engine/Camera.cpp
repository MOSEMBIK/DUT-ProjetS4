#include <Engine/Camera.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

Camera::Camera() : m_transform(Transform()), m_fov(60), m_nearPlane(0.03f), m_farPlane(100.0f)
{

}

mat4 Camera::getViewMatrix() const
{
    mat4 translate = glm::translate(m_transform.getPosition());
    mat4 rotate = glm::toMat4(m_transform.getRotation());

    return rotate * translate;
}

mat4 Camera::getProjectionMatrix(GLFWwindow* window) const
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

	return perspective(radians(m_fov), (float)width / height, m_nearPlane, m_farPlane);
}