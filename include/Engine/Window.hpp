#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>

#include <glm/vec2.hpp>

const unsigned int WINDOW_W = 1280;
const unsigned int WINDOW_H = 720;

class Window
{
private:
    GLFWwindow* m_window;
	std::map<int, std::function<void(double, double, int)>> m_callbacks;
    glm::ivec2 m_size;
	int CALLBACK_ID = 0;

public:
	glm::vec2 m_scale;

    Window();
    // Suppression du clonage et de l'opÃ©rateur =
    Window(Window&) = delete;
    void operator= (const Window&) = delete;

    inline GLFWwindow* getWindow() const { return m_window; } 
    inline glm::ivec2 getSize() { return m_size; }

    int registerCallback(std::function<void(double, double, int)> callback);
	void unregisterCallback(int i);
    void update();
};
