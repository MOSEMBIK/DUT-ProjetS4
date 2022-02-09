#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VAO/IndexedVAO.h>

namespace Renderer::Vertex {
    class IndexedDraw {
    protected:
        IndexedDrawCommand m_cmd;
        VAO::IndexedVAO m_vao;
    public:
        IndexedDraw(const VertexParams& v, const ElementParams& e, const IndexedDrawCommand& cmd)
            : m_vao(VAO::IndexedVAO(v, e)), m_cmd(cmd)
        {
            m_cmd.m_offset = 0;
        }

        void Draw(GLsizei primCount = 1) const
        {
            m_vao.Draw({ m_cmd.m_mode, m_cmd.m_count, m_cmd.m_offset, primCount });
        }
    };
}