#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VAO/AbstractVAO.h>
#include <Renderer/Vertex/VBO/ArrayBuffer.h>

namespace Renderer::Vertex::VAO {
    class DirectVAO : public AbstractVAO {
    protected:
        VBO::ArrayBuffer m_vertices;
    public:
        DirectVAO(const VertexParams& v) 
            : m_vertices(VBO::ArrayBuffer(&v.m_data[0], v.m_data.size() * sizeof(GLfloat), v.m_attribs, v.m_usage))
        {
            Bind();
            m_vertices.Bind();
            Unbind();
        }

        void Draw(const DirectDrawCommand& cmd) const
        {
            Bind();
            glDrawArraysInstanced(cmd.m_mode, cmd.m_first, cmd.m_count, cmd.m_primCount);
            Unbind();
        }
    };
}