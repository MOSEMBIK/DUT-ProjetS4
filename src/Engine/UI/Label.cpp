#include <Engine/UI/Label.hpp>

using namespace glm;
using namespace std;

Label::Label(Window* window, vec2 position, string text, char* font, float fontHeight, vec3 fontColor) : Widget(window, position),
    m_text(text), m_fontHeight(fontHeight), m_fontColor(fontColor), m_textRenderer(TextRenderer(1280, 720))
{
    m_textRenderer.loadFont(font, fontHeight);
}

void Label::setFont(char* font)
{
    m_textRenderer.loadFont(font, m_fontHeight);
}

void Label::draw()
{
    m_textRenderer.renderText(m_text, m_position.x, m_position.y, 1.0f, m_fontColor);
}
