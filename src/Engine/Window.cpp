#include <Engine/Window.hpp>

Window* Window::m_instance= nullptr;

Window::Window() : m_window(glfwCreateWindow(WINDOW_W, WINDOW_H, "Game", NULL, NULL))
{

}

Window* Window::GetMain() 
{
    if(m_instance == nullptr)
    {
        m_instance = new Window();
    }
    return m_instance;
}
