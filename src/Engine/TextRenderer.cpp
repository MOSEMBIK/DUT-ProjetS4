#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <Engine/TextRenderer.hpp>

using namespace std;
using namespace glm;

TextRenderer::TextRenderer(Window* window) : m_textShader(Shader("shader/ui/text/vertex.glsl", "shader/ui/text/fragment.glsl")), m_window(window)
{
    // configure shader
	m_textShader.use();
    m_textShader.setUniformValue("u_text", 0);
    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::loadFont(string font, unsigned int fontSize)
{
    // first clear the previously loaded Characters
    m_characters.clear();
    // then initialize and load the FreeType library
    FT_Library ft;    
    if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        cout << "ERROR::FREETYPE: Failed to load font" << endl;
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    // then for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (uint)face->glyph->advance.x
        };
        m_characters.insert(pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

float TextRenderer::getTextWidth(string text)
{
    float width = 0.0f;
    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];
        width += (ch.m_advance >> 6);
    }
    return width;
}

float TextRenderer::getTextHeight(string text)
{
    float height = 0.0f;
    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];
        if(height < ch.m_size.y)
            height = ch.m_size.y;
    }
    return height;
}

void TextRenderer::renderText(string text, vec2 position, vec2 anchor, int align, float scale, vec3 color)
{
    // activate corresponding render state	
    m_textShader.use();
	mat4 P = ortho(-(float)m_window->getWidth() * anchor.x, (float)m_window->getWidth() * (1 - anchor.x), -(float)m_window->getHeight() * anchor.y, (float)m_window->getHeight() * (1 - anchor.y));
	m_textShader.setUniformValue("u_P", P);
    m_textShader.setUniformValue("u_textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    if(align & ALIGN_CENTER)
    {
        position.x -= getTextWidth(text) / 4.0f;
    }
    else if(align & ALIGN_RIGHT)
    {
        position.x -= getTextWidth(text);
    }

    if(align & ALIGN_MIDDLE)
    {
        position.y -= getTextHeight(text) / 4.0f;
    }
    else if(align & ALIGN_TOP)
    {
        position.y -= getTextHeight(text);
    }
	position *= m_window->getScale().y;

    // iterate through all characters
    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float xpos = position.x + ch.m_bearing.x * scale;
        float ypos = position.y + (ch.m_size.y - ch.m_bearing.y) * scale;

        float w = ch.m_size.x * scale;
        float h = ch.m_size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.m_textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        position.x += (ch.m_advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
