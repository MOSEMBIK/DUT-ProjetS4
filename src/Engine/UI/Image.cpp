#include <Engine/UI/Image.hpp>

#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

#include <glm/gtx/transform.hpp>
#include <Game/Game.hpp>

using namespace glm;
using namespace std;

Mesh* Image::imageQuad;
Shader* Image::imageShader;

Image::Image(Window* window, vec2 position, vec2 anchor, vec2 size, Texture* texture, vec3 color)
    : Widget(window, position, anchor), m_size(size), m_texture(texture), m_color(color)
{

}

void Image::init()
{

}

void Image::draw()
{
    if(imageShader == nullptr)
    {
        imageShader = new Shader("shader/ui/image/vertex.glsl", "shader/ui/image/fragment.glsl");
    }

    if(imageQuad == nullptr)
    {
        imageQuad = Primitives::quad();
    }

	mat4 M = translate(vec3(m_position.x + (0.5f - m_anchor.x) * m_size.x, m_position.y + (0.5f - m_anchor.y) * m_size.y, 0.0f)) * scale(vec3(m_size.x, -m_size.y, 1));
	mat4 P = ortho(-(float)m_window->getSize().x * m_anchor.x, (float)m_window->getSize().x * (1 - m_anchor.x), -(float)m_window->getSize().y * m_anchor.y, (float)m_window->getSize().y * (1 - m_anchor.y));

	imageShader->use();
	imageShader->setUniformValue("u_M", M);
	imageShader->setUniformValue("u_P", P);

    glActiveTexture(GL_TEXTURE0);
    imageShader->setUniformValue("u_imageTexture", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture->m_id);
    imageShader->setUniformValue("u_color", m_color);

	imageQuad->draw();
}
