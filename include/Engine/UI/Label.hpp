#pragma once

#include <Engine/UI/Widget.hpp>
#include <Engine/TextRenderer.hpp>

#include <glm/vec3.hpp>

#include <string>

class Label : public Widget {
protected:
    std::string m_text;
    float m_fontHeight;
    glm::vec3 m_fontColor;
    TextRenderer m_textRenderer;

public:
    Label(glm::vec2 position, std::string text, char* font = "assets/fonts/arial.ttf", float fontHeight = 24, glm::vec3 fontColor = glm::vec3(1.0f));

    inline void setText(std::string text) { m_text = text; }
    void setFont(char* font);
    inline void setFontHeight(float fontHeight) { m_fontHeight = fontHeight; }
    inline void setFontColor(glm::vec3 fontColor) { m_fontColor = fontColor; } 

    inline std::string getText() const { return m_text; }
    inline float getFontHeight() const { return m_fontHeight; }
    inline glm::vec3 getFontColor() const { return m_fontColor; } 

    virtual void draw();
};
