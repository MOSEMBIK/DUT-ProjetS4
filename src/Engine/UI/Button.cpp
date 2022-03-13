#include <Engine/UI/Button.hpp>

#include <Game/Game.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/vec4.hpp>
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
	window->attachEventReceiver(*this);
	Resource::loadTexture(texture, m_texture);
	Resource::loadTexture(clickedTexture, m_clickedTexture);
	Resource::loadTexture(highlightedTexture, m_highlightedTexture);
}

Button::Button(Window* window, vec2 position, vec2 anchor, vec2 size, char* texture, char* clickedTexture,
	vec3 color, vec3 clickedColor, vec3 highlightedColor)
	: Widget(window, position, anchor), m_state(State::NONE), m_label(window, vec2(0.0f), vec2(0.0f), ""), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor)
{
	window->attachEventReceiver(*this);
	Resource::loadTexture(texture, m_texture);
	Resource::loadTexture(clickedTexture, m_clickedTexture);
	m_highlightedTexture = m_texture;
}

Button::Button(Window* window, vec2 position, vec2 anchor, vec2 size, char* texture,
	vec3 color, vec3 clickedColor, vec3 highlightedColor)
	: Widget(window, position, anchor), m_state(State::NONE), m_label(window, vec2(0.0f), vec2(0.0f), ""), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor)
{
	window->attachEventReceiver(*this);
	Resource::loadTexture(texture, m_texture);
	m_clickedTexture = m_highlightedTexture = m_texture;
}

Button::~Button()
{
	m_window->detachEventReceiver(*this);
}


void Button::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Button::onMouseMoved, this, std::placeholders::_1));
	dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Button::onMouseButtonPressed, this, std::placeholders::_1));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Button::onMouseButtonReleased, this, std::placeholders::_1));
}

void Button::draw()
{
	mat4 M = translate(vec3(m_position.x, m_position.y, 0.0f) * m_window->getScale().y) * scale(vec3(m_size.x, -m_size.y, 1) * m_window->getScale().y);	
	mat4 P = ortho(-(float)m_window->getWidth() * m_anchor.x, (float)m_window->getWidth() * (1 - m_anchor.x), -(float)m_window->getHeight() * m_anchor.y, (float)m_window->getHeight() * (1 - m_anchor.y));

	Shader* shader;
	if(m_nineSlice > 0)
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
			glBindTexture(GL_TEXTURE_2D, m_texture->m_id);
			shader->setUniformValue("u_color", m_color);
			break;

		case State::HOVERED:
			if(!m_clicked)
			{
				glBindTexture(GL_TEXTURE_2D, m_highlightedTexture->m_id);
				shader->setUniformValue("u_color", m_highlightedColor);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_clickedTexture->m_id);
				shader->setUniformValue("u_color", m_clickedColor);
			}
			break;
	}

	if (m_nineSlice > 0) {
		float slice = m_nineSlice;
		shader->setUniformValue("u_dimensions", vec2(slice / m_size.x, slice / m_size.y));
		shader->setUniformValue("u_border",  vec2(slice / m_texture->m_width, slice / m_texture->m_height));
	}

	buttonQuad->draw();

	if (m_label.getText() != "") {
		m_label.draw();
	}
}

bool Button::onMouseMoved(MouseMovedEvent& e)
{
	float scale = m_window->getScale().y;

	if(e.GetX() >= (m_position.x * scale + (float)m_window->getWidth() * m_anchor.x) - m_size.x * 0.5f * scale
	&& e.GetX() < (m_position.x * scale + (float)m_window->getWidth() * m_anchor.x) + m_size.x * 0.5f * scale
	&& m_window->getHeight() - e.GetY() >= (m_position.y * scale + (float)m_window->getHeight() * m_anchor.y) - m_size.y * 0.5f * scale
	&& m_window->getHeight() - e.GetY() < (m_position.y * scale + (float)m_window->getHeight() * m_anchor.y) + m_size.y * 0.5f * scale) 
	{
		m_state = State::HOVERED;
	}
	else
	{
		m_state = State::NONE;
	}

	return false;
}

bool Button::onMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if(m_state == State::HOVERED)
	{
		if(e.GetMouseButton() == 0)
		{
			m_clicked = true;
		}
	}

	return false;
}

bool Button::onMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	bool catched = false;
	if(m_clicked && m_state == State::HOVERED && e.GetMouseButton() == 0)
	{
		catched = true;
		onClick();
	}
	m_clicked = false;

	return catched;
}