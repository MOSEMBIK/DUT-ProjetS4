#include <Engine/UI/Label.hpp>

using namespace glm;
using namespace std;

Label::Label(Window* window, vec2 position, vec2 anchor, string text, float fontHeight, char* font, int alignment, vec3 fontColor) : Widget(window, position, anchor),
    m_text(text), m_fontHeight(fontHeight), m_font(font), m_alignment(alignment), m_fontColor(fontColor), m_textRenderer(TextRenderer(window))
{
    m_textRenderer.loadFont(font, fontHeight);
}

void Label::setFont(char* font)
{
    m_textRenderer.loadFont(font, m_fontHeight);
}

void Label::setFontHeight(float fontHeight)
{
	m_fontHeight = fontHeight;
	m_textRenderer.loadFont(m_font, m_fontHeight);
}

void Label::draw()
{
    m_textRenderer.renderText(m_text, m_position, m_anchor, m_alignment, 1.0f, m_fontColor);
}
