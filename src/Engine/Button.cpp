
#include <Engine/Button.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

Button::Button(float width, float height, float x, float y, const char* text, GLFWwindow* window, const char* texture) : m_width(width), m_height(height), m_x(x), m_y(y), m_text(text), m_mesh(Primitives::Quad(vec2(width, height))) {
	if (!Resource::LoadTexture(texture, m_texture)) {
		cout << "Failed to load texture " << texture << endl;
		return;
	}

	shader = new Shader("shader/button_vertex.glsl", "shader/button_fragment.glsl");
}

void Button::draw() {
	shader->Use();
	mat4 M = translate(vec3(m_x, m_y, 0.0f));
	shader->SetUniformValue("_M", M);
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	m_mesh.Draw();
}
