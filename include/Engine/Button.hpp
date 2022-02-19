#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Engine/Mesh.hpp>

#include <functional>

class Button
{
private:
	float m_width, m_height, m_x, m_y;
	unsigned int m_texture;
	const char* m_text;
	Mesh m_mesh;
	Shader * shader;

public:
    Button(float width, float height, float x, float y, const char* text, GLFWwindow* window, const char* texture = "assets/button.png");

	void draw();

	std::function<void()> onClick;
};
