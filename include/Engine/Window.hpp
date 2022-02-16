#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    const unsigned int WINDOW_W = 1920;
    const unsigned int WINDOW_H = 1080;

    Window();
    GLFWwindow* m_window;

    // Singleton
    static Window* m_instance;

public:
    // Suppression du clonage et de l'opÃ©rateur =
    Window(Window&) = delete;
    void operator= (const Window&) = delete;

    /**
     * @brief Get the main Window
     * 
     * @return Window* 
     */
    static Window* GetMain();

    inline GLFWwindow* GetWindow() const { return m_window; } 
};
