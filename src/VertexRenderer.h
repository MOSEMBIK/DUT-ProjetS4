#pragma once

#include <vector>
#include <functional>

namespace VertexRenderer {
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

	struct DirectDrawCommand
	{
		GLenum m_mode;
		GLint m_first = 0;
		GLsizei m_count;
		GLsizei m_primCount = 0;
	};

	namespace VBO {

		class AbstractVBO
		{
		protected:
			GLuint m_target;
			GLuint m_rendererID;
		public:
			AbstractVBO(const void*, unsigned int, int = GL_ARRAY_BUFFER, GLenum = GL_STATIC_DRAW);
			void Bind(std::function<void(const AbstractVBO*)>, bool = true) const;
			virtual void Draw(const DirectDrawCommand&) const = 0;
		};

		class ArrayBuffer : public AbstractVBO {
		protected:
			std::vector<VertexAttri> m_attribs;
		public:
			ArrayBuffer(const void*, unsigned int, const std::vector<VertexAttri>&, GLenum = GL_STATIC_DRAW);
			void Bind(std::function<void(const ArrayBuffer*)>, bool = true) const;
			virtual void Draw(const DirectDrawCommand&) const;
		};
	}

	namespace VAO {
		struct VertexParams
		{
			std::vector<GLfloat> m_data;
			std::vector<VertexAttri> m_attribs;
			GLenum m_usage = GL_STATIC_DRAW;
		};

		class AbstractVAO
		{
		protected:
			GLuint m_vaoId;
		public:
			AbstractVAO();
			void Bind(std::function<void(const AbstractVAO*)>) const;
			virtual void Draw(const DirectDrawCommand&) const = 0;
		};

		class DirectVAO : public AbstractVAO {
		protected:
			VBO::ArrayBuffer m_vertices;
		public:
			DirectVAO(const VertexParams&);
			virtual void Draw(const DirectDrawCommand&) const;
		};

		class DirectDraw {
		protected:
			DirectVAO m_vao;
			DirectDrawCommand m_cmd;
		public:
			DirectDraw(const VertexParams&, const DirectDrawCommand&);
			virtual void Draw(GLsizei = 1) const;
		};
	}
}