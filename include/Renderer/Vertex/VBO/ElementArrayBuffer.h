#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VBO/AbstractVBO.h>

namespace Renderer::Vertex::VBO {
    class ElementArrayBuffer : public AbstractVBO {
    public:
        const GLenum m_type;

        ElementArrayBuffer(const void* data, unsigned int size, GLenum type = GL_UNSIGNED_INT, GLenum usage = GL_STATIC_DRAW)
            : AbstractVBO(data, size, GL_ELEMENT_ARRAY_BUFFER, usage)
            , m_type(type)
        {
            
        }

        void Draw(const IndexedDrawCommand& cmd) const
        {
            Bind();
            glDrawElements(cmd.m_mode, cmd.m_count, m_type, nullptr);
            Unbind();
        } 
};
}