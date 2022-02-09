#pragma once

#include <vector>
#include <functional>

namespace Renderer::Vertex {
	struct VertexAttri
	{
		GLuint m_index;
		GLint m_size;
		GLenum m_type = GL_FLOAT;
		GLboolean m_normalized = false;
		GLsizei m_stride = 0;
		GLint m_offset = 0;
		GLuint m_divisor = 0;
	};

	struct DrawCommand
	{
		GLenum m_mode;
		GLsizei m_count;
	};
	struct DirectDrawCommand : DrawCommand
	{
		GLint m_first = 0;
		GLsizei m_primCount = 1;
	};

	struct IndexedDrawCommand : DrawCommand
	{
		GLint m_offset = 0;
		GLsizei m_primCount = 1;
	};
		
	struct VertexParams
	{
		std::vector<GLfloat> m_data;
		std::vector<VertexAttri> m_attribs;
		GLenum m_usage = GL_STATIC_DRAW;
	};

	struct ElementParams
	{
		std::vector<GLuint> m_data;
		GLenum m_type = GL_UNSIGNED_INT;
		GLenum m_usage = GL_STATIC_DRAW;
	};
}
