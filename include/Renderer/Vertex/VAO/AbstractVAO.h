#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VertexRenderer.h>

namespace Renderer::Vertex::VAO {
    class AbstractVAO {
    protected:
        GLuint m_vaoId;
    public:
        AbstractVAO()
        {
            glCreateVertexArrays(1, &m_vaoId);
        }

        void Bind() const
        {
            glBindVertexArray(m_vaoId);
        }

        void Unbind() const
        {
            glBindVertexArray(0);
        }
            
        virtual void Draw(const DirectDrawCommand&) const { }
    };
}