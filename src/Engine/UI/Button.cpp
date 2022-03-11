#include <Engine/UI/Button.hpp>

#include <Game/Game.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/gtx/transform.hpp>

using namespace glm;
using namespace std;

Mesh* Button::buttonQuad;
Shader* Button::buttonShader;
Shader* Button::buttonSlicedShader;

// TODO : Recentralisé cette partie du code

Button::Button(Window* window, vec2 position, vec2 anchor, vec2 size, char* texture, char* clickedTexture, char* highlightedTexture,
	vec3 color, vec3 clickedColor, vec3 highlightedColor)
	: Widget(window, position, anchor), m_state(State::NONE), m_label(window, vec2(0.0f), vec2(0.0f), ""), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor)
{
	Resource::loadTexture(texture, m_texture);
	Resource::loadTexture(clickedTexture, m_clickedTexture);
	Resource::loadTexture(highlightedTexture, m_highlightedTexture);

	init();
}

Button::Button(Window* window, vec2 position, vec2 anchor, vec2 size, char* texture, char* clickedTexture,
	vec3 color, vec3 clickedColor, vec3 highlightedColor)
	: Widget(window, position, anchor), m_state(State::NONE), m_label(window, vec2(0.0f), vec2(0.0f), ""), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor)
{
	Resource::loadTexture(texture, m_texture);
	Resource::loadTexture(clickedTexture, m_clickedTexture);
	m_highlightedTexture = m_texture;

	init();
}

Button::Button(Window* window, vec2 position, vec2 anchor, vec2 size, char* texture,
	vec3 color, vec3 clickedColor, vec3 highlightedColor)
	: Widget(window, position, anchor), m_state(State::NONE), m_label(window, vec2(0.0f), vec2(0.0f), ""), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor)
{
	Resource::loadTexture(texture, m_texture);
	m_clickedTexture = m_highlightedTexture = m_texture;

	init();
}


void Button::init()
{
	// TODO : Créer un callback qui gère le click, et l'action du click etc...
	this->m_clickCallbackId = m_window->registerCallback([this](double xPos, double yPos, int click) {
		float scale = m_window->m_scale.y;
		if(xPos >= (m_position.x * scale + (float)m_window->getSize().x * m_anchor.x) - m_size.x * 0.5f * scale
		&& xPos < (m_position.x * scale + (float)m_window->getSize().x * m_anchor.x) + m_size.x * 0.5f * scale
		&& yPos >= (m_position.y * scale + (float)m_window->getSize().y * m_anchor.y) - m_size.y * 0.5f * scale
		&& yPos < (m_position.y * scale + (float)m_window->getSize().y * m_anchor.y) + m_size.y * 0.5f * scale) {
			if (click)
				m_state = State::CLICKED;
			else {
				if (m_state == State::CLICKED)
					onClick();
				m_state = State::HIGHLIGHTED;
			}
		}
		else
			m_state = State::NONE;
	});
}

void Button::draw()
{
	mat4 M = translate(vec3(m_position.x, m_position.y, 0.0f) * m_window->m_scale.y) * scale(vec3(m_size.x, -m_size.y, 1) * m_window->m_scale.y);	
	mat4 P = ortho(-(float)m_window->getSize().x * m_anchor.x, (float)m_window->getSize().x * (1 - m_anchor.x), -(float)m_window->getSize().y * m_anchor.y, (float)m_window->getSize().y * (1 - m_anchor.y));

	Shader* shader;
	if(m_nineSlice)
	{
		if (buttonSlicedShader == nullptr) {
			buttonSlicedShader = Shader::find("UI/Slicing");
			if(buttonSlicedShader == nullptr)  {
				buttonSlicedShader = new Shader("shader/ui/slicing/vertex.glsl", "shader/ui/slicing/fragment.glsl");
				Shader::save("UI/Slicing", buttonSlicedShader);
			}
		}
		shader = buttonSlicedShader;
	}

	else
	{
		if (buttonShader == nullptr) {
			buttonShader = Shader::find("UI/Image");
			if(buttonShader == nullptr) {
				buttonShader = new Shader("shader/ui/image/vertex.glsl", "shader/ui/image/fragment.glsl");
				Shader::save("UI/Image", buttonShader);
			}
		}
		shader = buttonShader;
	}

	if(buttonQuad == nullptr)
	{
		buttonQuad = Primitives::quad();
	}

	shader->use();
	shader->setUniformValue("u_M", M);
	shader->setUniformValue("u_P", P);

	glActiveTexture(GL_TEXTURE0);
	shader->setUniformValue("u_imageTexture", 0);
	switch(m_state)
	{
		case State::NONE:
			glBindTexture(GL_TEXTURE_2D, m_texture.m_id);
			shader->setUniformValue("u_color", m_color);
			break;

		case State::HIGHLIGHTED:
			glBindTexture(GL_TEXTURE_2D, m_highlightedTexture.m_id);
			shader->setUniformValue("u_color", m_highlightedColor);
			break;

		case State::CLICKED:
			glBindTexture(GL_TEXTURE_2D, m_clickedTexture.m_id);
			shader->setUniformValue("u_color", m_clickedColor);
			break;
	}

	if (m_nineSlice) {
		shader->setUniformValue("u_dimensions", vec2(1.0f / m_texture.m_width, 1.0f / m_texture.m_height * m_size.x / m_size.y));
		shader->setUniformValue("u_border", vec2(0.25f, 0.25f));
	}

	buttonQuad->draw();

	if (m_label.getText() != "") {
		m_label.draw();
	}
}
