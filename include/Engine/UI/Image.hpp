#pragma once

#include <Engine/UI/Widget.hpp>
#include <Engine/UI/Label.hpp>

#include <Engine/Shader.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Mesh.hpp>

#include <glm/vec3.hpp>

#include <functional>

class Image : public Widget {
protected:
    static Mesh* imageQuad;
    static Shader* imageShader;

    glm::vec2 m_size;

    Texture* m_texture;
    glm::vec3 m_color;

public:
    Image(Window* window, glm::vec2 position, glm::vec2 anchor, glm::vec2 size, Texture* texture, glm::vec3 color = glm::vec3(1.0f));

    inline void setSize(glm::vec2 size) { m_size = size; }
    inline void setTexture(Texture* texture) { m_texture = texture; }
    inline void setColor(glm::vec3 color) { m_color = color; } 

    inline glm::vec2 getSize() const { return m_size; } 
    inline Texture* getTexture() const { return m_texture; }
    inline glm::vec3 getColor() const { return m_color; }

    void init();
    void draw();
};
