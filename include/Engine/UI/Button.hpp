#pragma once

#include <Engine/UI/Widget.hpp>

#include <Engine/Shader.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Mesh.hpp>

#include <glm/vec3.hpp>

#include <functional>

class Button : public Widget {
    enum State { NONE, HIGHLIGHTED, CLICKED};

protected:
    static Mesh* buttonQuad;
    static Shader* buttonShader;

    State m_state;

    glm::vec2 m_size;

    Texture m_texture;
    glm::vec3 m_color;
    Texture m_clickedTexture;
    glm::vec3 m_clickedColor;
    Texture m_highlightedTexture;
    glm::vec3 m_highlightedColor;

public:
    Button(glm::vec2 position, glm::vec2 size, char* texture, char* clickedTexture, char* highlightedTexture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    Button(glm::vec2 position, glm::vec2 size, char* texture, char* clickedTexture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    Button(glm::vec2 position, glm::vec2 size, char* texture,
    glm::vec3 color = glm::vec3(1.0f), glm::vec3 clickedColor = glm::vec3(1.0f), glm::vec3 highlightedColor = glm::vec3(1.0f));

    inline void setSize(glm::vec2 size) { m_size = size; }

    inline void setTexture(Texture texture) { m_texture = texture; }
    inline void setClickedTexture(Texture texture) { m_clickedTexture = texture; }
    inline void setHighlightedTexture(Texture texture) { m_highlightedTexture = texture; }

    inline void setColor(glm::vec3 color) { m_color = color; } 
    inline void setClickedColor(glm::vec3 color) { m_clickedColor = color; } 
    inline void setHighlightedColor(glm::vec3 color) { m_highlightedColor = color; } 

    inline glm::vec2 getSize() { return m_size; } 

    inline Texture getTexture() const { return m_texture; }
    inline Texture getClickedTexture() const { return m_clickedTexture; }
    inline Texture getHighlightedTexture() const { return m_highlightedTexture; }

    inline glm::vec3 getColor() const { return m_color; }
    inline glm::vec3 getClickedColor() const { return m_clickedColor; }
    inline glm::vec3 getHighlightedColor() const { return m_highlightedColor; }

    virtual void draw();
};
