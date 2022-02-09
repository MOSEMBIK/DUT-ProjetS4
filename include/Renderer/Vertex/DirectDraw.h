#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Vertex/VAO/DirectVAO.h>

namespace Renderer::Vertex {
    class DirectDraw {
    protected:
        VAO::DirectVAO m_vao;
        DirectDrawCommand m_cmd;
    public:
        DirectDraw(const VertexParams& v, const DirectDrawCommand& cmd)
            : m_vao(VAO::DirectVAO(v)), m_cmd(cmd)
        {
            this->m_cmd.m_first = 0;
        }

        void Draw(GLsizei primCount = 1) const
        {
            this->m_vao.Draw({ m_cmd.m_mode, m_cmd.m_count, m_cmd.m_first, primCount });
        }
    };
}