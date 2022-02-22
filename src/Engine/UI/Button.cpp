#include <Engine/UI/Button.hpp>

#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/gtx/transform.hpp>

using namespace glm;
using namespace std;

Mesh* Button::buttonQuad;
Shader* Button::buttonShader;

Button::Button(vec2 position, vec2 size, char* texture, char* clickedTexture, char* highlightedTexture,
    vec3 color, vec3 clickedColor, vec3 highlightedColor)
    : Widget(position), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor), m_state(State::NONE)
{
    Resource::LoadTexture(texture, m_texture);
    Resource::LoadTexture(clickedTexture, m_clickedTexture);
    Resource::LoadTexture(highlightedTexture, m_highlightedTexture);
}

Button::Button(vec2 position, vec2 size, char* texture, char* clickedTexture,
    vec3 color, vec3 clickedColor, vec3 highlightedColor)
    : Widget(position), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor), m_state(State::NONE)
{
    Resource::LoadTexture(texture, m_texture);
    Resource::LoadTexture(clickedTexture, m_clickedTexture);
    m_highlightedTexture = m_texture;
}

Button::Button(vec2 position, vec2 size, char* texture,
    vec3 color, vec3 clickedColor, vec3 highlightedColor)
    : Widget(position), m_size(size), m_color(color), m_clickedColor(clickedColor), m_highlightedColor(highlightedColor), m_state(State::NONE)
{
    Resource::LoadTexture(texture, m_texture);
    m_clickedTexture = m_highlightedTexture = m_texture;
}


void Button::draw()
{
    if(buttonShader == nullptr)
    {
        buttonShader = Shader::Find("UI/Slicing");
        if(buttonShader == nullptr)
        {
            buttonShader = new Shader("shader/ui/slicing/vertex.glsl", "shader/ui/slicing/fragment.glsl");
            Shader::Register("UI/Slicing", buttonShader);
        }
    }

    if(buttonQuad == nullptr)
    {
        buttonQuad = Primitives::Quad();
    }

	mat4 M = translate(vec3(m_position.x + m_size.x / 2.0f, m_position.y + m_size.y / 2.0f, 0.0f)) * scale(vec3(m_size.x, m_size.y, 1));	
	mat4 P = ortho(0.0f, 1280.0f, 0.0f, 720.0f);

	buttonShader->Use();
	buttonShader->SetUniformValue("u_M", M);
	buttonShader->SetUniformValue("u_P", P);
	
    glActiveTexture(GL_TEXTURE0);
    buttonShader->SetUniformValue("u_buttonTexture", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture.m_id);

    buttonShader->SetUniformValue("u_dimensions", vec2(1.0f / m_texture.m_width, 1.0f / m_texture.m_height * m_size.x / m_size.y));
    buttonShader->SetUniformValue("u_border", vec2(0.25f, 0.25f));

	buttonQuad->Draw();
}
