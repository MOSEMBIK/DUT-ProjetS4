#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VertexRenderer.h>

namespace Renderer::Vertex::VBO {
    class AbstractVBO
    {
    protected:
        GLuint m_target;
        GLuint m_rendererID;
    public:
        AbstractVBO(const void* data, unsigned int size, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) 
        : m_target(target)
        {
            glCreateBuffers(1, &m_rendererID);

            glBindBuffer(target, m_rendererID);
            glBufferData(target, size, data, GL_STATIC_DRAW);
            glBindBuffer(target, 0);
        }

        void Bind() const
        {
            glBindBuffer(m_target, m_rendererID);
        }

        void Unbind() const
        {
            glBindBuffer(m_target, 0);
        }
        
        virtual void Draw(const DirectDrawCommand&) const { }
    };
}