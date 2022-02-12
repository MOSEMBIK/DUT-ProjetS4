#include <Engine/Camera.hpp>

using namespace glm;

Camera* Camera::m_instance= nullptr;;

Camera::Camera() : m_transform(Transform()), m_fov(60), m_nearPlane(0.03f), m_farPlane(100.0f)
{

}

Camera* Camera::GetInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new Camera();
    }
    return m_instance;
}

mat4 Camera::GetProjectionMatrix(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

	return perspective(radians(m_fov), (float)width / height, m_nearPlane, m_farPlane);
}