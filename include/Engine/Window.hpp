#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include <glm/vec2.hpp>

const unsigned int WINDOW_W = 1280;
const unsigned int WINDOW_H = 720;

class Window
{
private:
    GLFWwindow* m_window;
    std::vector<std::function<void(double, double, int)>> m_callbacks;
    glm::ivec2 m_size;

public:
    Window();
    // Suppression du clonage et de l'opÃ©rateur =
    Window(Window&) = delete;
    void operator= (const Window&) = delete;

    inline GLFWwindow* getWindow() const { return m_window; } 
    inline glm::ivec2 getSize() { return m_size; }

    void registerCallback(std::function<void(double, double, int)> callback);
    void update();
};
