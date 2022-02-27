#include <Engine/UI/Button.hpp>

#include <Game/Game.hpp>
#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/gtx/transform.hpp>

using namespace glm;
using namespace std;

Mesh* Button::buttonQuad;
Shader* Button::buttonShader;

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
    m_window->registerCallback([this](double xPos, double yPos, int click)
    {
        if(xPos >= (m_position.x + (float)m_window->getSize().x * m_anchor.x) - m_anchor.x * m_size.x
        && xPos < (m_position.x + (float)m_window->getSize().x * m_anchor.x) + (1.0f - m_anchor.x) * m_size.x
        && yPos >= (m_position.y + (float)m_window->getSize().y * m_anchor.y) - m_anchor.y * m_size.y
        && yPos < (m_position.y + (float)m_window->getSize().y * m_anchor.y) + (1.0f - m_anchor.y) * m_size.y)
        {
            if(click) {
				if (m_state != State::CLICKED)
                	onClick();
            }
			else {
				onHover();
			}
        }
    });
}

void Button::onClick()
{
	m_state = State::CLICKED;
	Game* game = Game::getInstance();
	switch(game->getState())
	{
		case GameState::MAIN_MENU:
			game->setState(GameState::GAME);
			break;
		case GameState::GAME:
			game->setState(GameState::MAIN_MENU);
			break;
	}
}

void Button::onHover()
{
	m_state = State::HIGHLIGHTED;
	//cout << "Hover" << endl;
}

void Button::draw()
{
    if(buttonShader == nullptr)
    {
        buttonShader = Shader::find("UI/Slicing");
        if(buttonShader == nullptr)
        {
            buttonShader = new Shader("shader/ui/slicing/vertex.glsl", "shader/ui/slicing/fragment.glsl");
            Shader::save("UI/Slicing", buttonShader);
        }
    }

    if(buttonQuad == nullptr)
    {
        buttonQuad = Primitives::quad();
    }

	mat4 M = translate(vec3(m_position.x + (0.5f - m_anchor.x) * m_size.x, m_position.y + (0.5f - m_anchor.y) * m_size.y, 0.0f)) * scale(vec3(m_size.x, m_size.y, 1));	
	mat4 P = ortho(-(float)m_window->getSize().x * m_anchor.x, (float)m_window->getSize().x * (1 - m_anchor.x), -(float)m_window->getSize().y * m_anchor.y, (float)m_window->getSize().y * (1 - m_anchor.y));

	buttonShader->use();
	buttonShader->setUniformValue("u_M", M);
	buttonShader->setUniformValue("u_P", P);
	
    glActiveTexture(GL_TEXTURE0);
    buttonShader->setUniformValue("u_buttonTexture", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture.m_id);

    buttonShader->setUniformValue("u_dimensions", vec2(1.0f / m_texture.m_width, 1.0f / m_texture.m_height * m_size.x / m_size.y));
    buttonShader->setUniformValue("u_border", vec2(0.25f, 0.25f));

	buttonQuad->draw();

	if (m_label.getText() != "") {
		m_label.draw();
	}
}
