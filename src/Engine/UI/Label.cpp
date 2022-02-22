#include <Engine/UI/Label.hpp>

using namespace glm;
using namespace std;

Label::Label(vec2 position, string text, char* font, float fontHeight, vec3 fontColor) : Widget(position),
    m_text(text), m_fontHeight(fontHeight), m_fontColor(fontColor), m_textRenderer(TextRenderer(1280, 720))
{
    m_textRenderer.Load(font, fontHeight);
}

void Label::setFont(char* font)
{
    m_textRenderer.Load(font, m_fontHeight);
}

void Label::draw()
{
    m_textRenderer.RenderText(m_text, m_position.x, m_position.y, 1.0f, m_fontColor);
}
