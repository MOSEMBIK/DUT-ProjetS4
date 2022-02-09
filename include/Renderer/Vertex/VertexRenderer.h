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

	namespace VBO {

		class AbstractVBO
		{
		protected:
			GLuint m_target;
			GLuint m_rendererID;
		public:
			const void* m_data;
			AbstractVBO(const void*, unsigned int, GLenum, GLenum = GL_STATIC_DRAW);
			virtual void Bind() const;
			void Unbind() const;
			virtual void Draw(const DirectDrawCommand&) const { }
		};

		class ArrayBuffer : public AbstractVBO {
		protected:
			std::vector<VertexAttri> m_attribs;
		public:
			ArrayBuffer(const void*, unsigned int, const std::vector<VertexAttri>&, GLenum = GL_STATIC_DRAW);
			virtual void Bind() const;
			virtual void Draw(const DirectDrawCommand&) const;
		};

		class ElementArrayBuffer : public AbstractVBO {
		public:
			const GLenum m_type;
			ElementArrayBuffer(const void*, unsigned int, GLenum = GL_UNSIGNED_INT, GLenum = GL_STATIC_DRAW);
			virtual void Draw(const IndexedDrawCommand&) const;
		};
	}

	namespace VAO {
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

		class AbstractVAO
		{
		protected:
			GLuint m_vaoId;
		public:
			AbstractVAO();
			void Bind() const;
			void Unbind() const;
			virtual void Draw(const DirectDrawCommand&) const { }
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
			void Draw(GLsizei = 1) const;
		};

		class IndexedVAO : public AbstractVAO {
		public:
		protected:
			VBO::ArrayBuffer m_vertices;
			VBO::ElementArrayBuffer m_elements;
		public:
			IndexedVAO(const VertexParams&, const ElementParams&);
			virtual void Draw(const IndexedDrawCommand&) const;
		};

		class IndexedDraw {
		protected:
			IndexedDrawCommand m_cmd;
			IndexedVAO m_vao;
		public:
			IndexedDraw(const VertexParams&, const ElementParams&,const IndexedDrawCommand&);
			void Draw(GLsizei = 1) const;
		};
	}
}
