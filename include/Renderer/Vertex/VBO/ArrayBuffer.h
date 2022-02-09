#pragma once

#include <vector>
#include <Renderer/Vertex/VBO/AbstractVBO.h>

namespace Renderer::Vertex::VBO {
    class ArrayBuffer : public AbstractVBO {
    protected:
        std::vector<VertexAttri> m_attribs;
    public:
        ArrayBuffer(const void* data, unsigned int size, const std::vector<VertexAttri>& attribs, GLenum usage = GL_STATIC_DRAW)
            : AbstractVBO(data, size, GL_ARRAY_BUFFER, usage)
            , m_attribs(attribs)
        {

        }

        void Bind() const
        {
            glBindBuffer(m_target, m_rendererID);
            for (int i = 0; i < m_attribs.size(); ++i)
            {
                auto attrib = VertexAttri(m_attribs[i]);
                attrib.m_index = i;
                attrib.m_divisor = 0;
                glEnableVertexAttribArray(attrib.m_index);
                glVertexAttribPointer(attrib.m_index, attrib.m_size, attrib.m_type, attrib.m_normalized, attrib.m_stride, nullptr);
                glVertexAttribDivisor(attrib.m_index, attrib.m_divisor);
            }
        }

        void Draw(const DirectDrawCommand& cmd) const
        {
            Bind();
            glDrawArrays(cmd.m_mode, cmd.m_first, cmd.m_count);
            Unbind();
        }
    };
}