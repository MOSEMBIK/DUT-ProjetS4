#include <Engine/Camera.hpp>
#include <Game/Game.hpp>
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

void Camera::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Camera::onMouseMoved, this, std::placeholders::_1));

	dispatcher.Dispatch<AppUpdateEvent>(std::bind(&Camera::onAppUpdate, this, std::placeholders::_1));

	dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Camera::onMouseButtonPressed, this, std::placeholders::_1));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Camera::onMouseButtonReleased, this, std::placeholders::_1));
}

bool Camera::onMouseMoved(MouseMovedEvent& e)
{
	if (m_enableRotation) {
		const float rotationSpeed = 0.003f;
		m_transform.rotate(vec3(0.0f, 1.0f, 0.0f) * m_transform.getRotation() * rotationSpeed * e.GetDeltaX());
		m_transform.rotate(vec3(1.0f, 0.0f, 0.0f) * m_transform.getRotation() * rotationSpeed * e.GetDeltaY());
	}

    return false;
}

bool Camera::onMouseButtonPressed(MouseButtonPressedEvent& e)
{
    if(e.GetMouseButton() == 1)
        m_enableRotation = true;

    return false;
}

bool Camera::onMouseButtonReleased(MouseButtonReleasedEvent& e)
{
    if(e.GetMouseButton() == 1)
        m_enableRotation = false;

    return false;
}

bool Camera::onAppUpdate(AppUpdateEvent& e)
{
    const float cameraSpeed = 5.0f;

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        setFOV(getFOV() - 1);
	}

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        setFOV(getFOV() + 1);
	}

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        m_transform.translate(vec3(0.0f, 0.0f, 1.0f) * m_transform.getRotation() * cameraSpeed * e.GetDeltaTime());
	}

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        m_transform.translate(vec3(0.0f, 0.0f, -1.0f) * m_transform.getRotation() * cameraSpeed * e.GetDeltaTime());
	}

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        m_transform.translate(vec3(1.0f, 0.0f, 0.0f) * m_transform.getRotation() * cameraSpeed * e.GetDeltaTime());
	}

    if (glfwGetKey(e.GetWindow()->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        m_transform.translate(vec3(-1.0f, 0.0f, 0.0f) * m_transform.getRotation() * cameraSpeed * e.GetDeltaTime());
	}

    return false;
}