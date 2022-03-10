#include <Engine/Window.hpp>
#include <iostream>

Window::Window() : m_window(glfwCreateWindow(WINDOW_W, WINDOW_H, "Game", NULL, NULL)), m_callbacks(), m_scale(1.0f)
{
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

int Window::registerCallback(std::function<void(double, double, int)> callback)
{
    m_callbacks[CALLBACK_ID] = callback;
	return CALLBACK_ID++;
}

void Window::unregisterCallback(int i)
{
	m_callbacks.erase(i);
}

void Window::update()
{
    glfwGetWindowSize(m_window, &m_size.x, &m_size.y);
	m_scale = glm::vec2(m_size.x / float(WINDOW_W), m_size.y / float(WINDOW_H));

    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    yPos = m_size.y - yPos;

    int leftClick = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	int size = m_callbacks.size();
	for (auto cb : m_callbacks) {
		cb.second(xPos, yPos, leftClick);
		if (size != m_callbacks.size())
			return;
	}
}
