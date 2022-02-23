#pragma once

#include <map>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Engine/Texture.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Window.hpp>

struct Character {
    unsigned int m_textureID;
    glm::ivec2   m_size;     
    glm::ivec2   m_bearing;  
    unsigned int m_advance;  
};

enum TextAlignement
{
    ALIGN_LEFT = 1,
    ALIGN_CENTER = 2,
    ALIGN_RIGHT = 4,
    ALIGN_TOP = 8,
    ALIGN_MIDDLE = 16,
    ALIGN_BOTTOM = 32
};

class TextRenderer
{
private:
    Shader m_textShader;
    std::map<char, Character> m_characters; 
    Window* m_window;
    unsigned int VAO, VBO;
    
public:
    TextRenderer(Window* window);
    void loadFont(std::string font, unsigned int fontSize);
    void renderText(std::string text, glm::vec2 position, glm::vec2 anchor, int alignment = (TextAlignement::ALIGN_LEFT | TextAlignement::ALIGN_BOTTOM), float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));

    float getTextWidth(std::string text);
    float getTextHeight(std::string text);
};