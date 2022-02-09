#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VAO/AbstractVAO.h>
#include <Renderer/Vertex/VBO/ArrayBuffer.h>
#include <Renderer/Vertex/VBO/ElementArrayBuffer.h>

namespace Renderer::Vertex::VAO {
    class IndexedVAO : public AbstractVAO {
    public:
    protected:
        VBO::ArrayBuffer m_vertices;
        VBO::ElementArrayBuffer m_elements;
    public:
        IndexedVAO(const VertexParams& v, const ElementParams& e) 
            : AbstractVAO()
            , m_vertices(VBO::ArrayBuffer(&v.m_data[0], v.m_data.size() * sizeof(GLfloat), v.m_attribs, v.m_usage))
            , m_elements(VBO::ElementArrayBuffer(&e.m_data[0], e.m_data.size() * sizeof(GLuint), e.m_type, e.m_usage))
        {
            Bind();
            m_vertices.Bind();
            m_elements.Bind();
            Unbind();
        }

        void Draw(const IndexedDrawCommand& cmd) const
        {
            Bind();
            glDrawElementsInstanced(cmd.m_mode, cmd.m_count, m_elements.m_type, nullptr, cmd.m_primCount);
            Unbind();
        }
    };
}