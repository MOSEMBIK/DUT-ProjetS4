#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int WINDOW_W = 1280;
const unsigned int WINDOW_H = 720;

class Window
{
private:
    GLFWwindow* m_window;

public:
    Window();
    
    // Suppression du clonage et de l'opÃ©rateur =
    Window(Window&) = delete;
    void operator= (const Window&) = delete;

    inline GLFWwindow* GetWindow() const { return m_window; } 
};
