#include <Engine/Window.hpp>

Window::Window() : m_window(glfwCreateWindow(WINDOW_W, WINDOW_H, "Game", NULL, NULL))
{

}