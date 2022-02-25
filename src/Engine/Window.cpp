#include <Engine/Window.hpp>
#include <iostream>

Window::Window() : m_window(glfwCreateWindow(WINDOW_W, WINDOW_H, "Game", NULL, NULL))
{
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void Window::registerCallback(std::function<void(double, double, int)> callback)
{
    m_callbacks.push_back(callback);
}

void Window::update()
{
    glfwGetWindowSize(m_window, &m_size.x, &m_size.y);

    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    yPos = m_size.y - yPos;

    int leftClick = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    for(auto cb : m_callbacks)
    {
        cb(xPos, yPos, leftClick); 
    }
}