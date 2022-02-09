#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include <Renderer/Vertex/VertexRenderer.h>
#include <iostream>
#include <iterator>

using namespace Renderer::Vertex;

VBO::AbstractVBO::AbstractVBO(const void* data, unsigned int size, GLenum target, GLenum usage) 
: m_target(target), m_data(m_data)
{
	glCreateBuffers(1, &m_rendererID);

	glBindBuffer(target, m_rendererID);
	glBufferData(target, size, data, GL_STATIC_DRAW);
	glBindBuffer(target, 0);
}

void VBO::AbstractVBO::Bind() const
{
	glBindBuffer(m_target, m_rendererID);
}

void VBO::AbstractVBO::Unbind() const
{
	glBindBuffer(m_target, 0);
}

void Attribs(const std::vector<VertexAttri>& attribs, unsigned int divisor = 0)
{
	for (int i = 0; i < attribs.size(); ++i)
	{
		auto attrib = VertexAttri(attribs[i]);
		attrib.m_index = i;
		attrib.m_divisor = divisor;
		glEnableVertexAttribArray(attrib.m_index);
		glVertexAttribPointer(attrib.m_index, attrib.m_size, attrib.m_type, attrib.m_normalized, attrib.m_stride, nullptr);
		glVertexAttribDivisor(attrib.m_index, attrib.m_divisor);
	}
}

VBO::ArrayBuffer::ArrayBuffer(const void* data, unsigned int size, const std::vector<VertexAttri>& attribs, GLenum usage)
	: AbstractVBO(data, size, GL_ARRAY_BUFFER, usage)
	, m_attribs(attribs)
{

}

void VBO::ArrayBuffer::Bind() const
{
	glBindBuffer(m_target, m_rendererID);
	Attribs(m_attribs);

}

void VBO::ArrayBuffer::Draw(const DirectDrawCommand& cmd) const
{
	Bind();
	glDrawArrays(cmd.m_mode, cmd.m_first, cmd.m_count);
	Unbind();
}

VAO::AbstractVAO::AbstractVAO()
{
	glCreateVertexArrays(1, &m_vaoId);
}

void VAO::AbstractVAO::Bind() const
{
	glBindVertexArray(m_vaoId);
}

void VAO::AbstractVAO::Unbind() const
{
	glBindVertexArray(0);
}

VAO::DirectVAO::DirectVAO(const VertexParams& v) 
	: m_vertices(VBO::ArrayBuffer(&v.m_data[0], v.m_data.size() * sizeof(GLfloat), v.m_attribs, v.m_usage))
{
	Bind();
	m_vertices.Bind();
	Unbind();
}

void VAO::DirectVAO::Draw(const DirectDrawCommand& cmd) const
{
	Bind();
	glDrawArraysInstanced(cmd.m_mode, cmd.m_first, cmd.m_count, cmd.m_primCount);
	Unbind();
}

VAO::DirectDraw::DirectDraw(const VertexParams& v, const DirectDrawCommand& cmd) : m_vao(DirectVAO(v)), m_cmd(cmd)
{
	this->m_cmd.m_first = 0;
}

void VAO::DirectDraw::Draw(GLsizei primCount) const
{
	this->m_vao.Draw({ m_cmd.m_mode, m_cmd.m_count, m_cmd.m_first, primCount });
}

VAO::IndexedVAO::IndexedVAO(const VertexParams& v, const ElementParams& e) 
	: AbstractVAO()
	, m_vertices(VBO::ArrayBuffer(&v.m_data[0], v.m_data.size() * sizeof(GLfloat), v.m_attribs, v.m_usage))
	, m_elements(VBO::ElementArrayBuffer(&e.m_data[0], e.m_data.size() * sizeof(GLuint), e.m_type, e.m_usage))
{
	Bind();
	m_vertices.Bind();
	m_elements.Bind();
	Unbind();
}

void VAO::IndexedVAO::Draw(const IndexedDrawCommand& cmd) const
{
	Bind();
	glDrawElementsInstanced(cmd.m_mode, cmd.m_count, m_elements.m_type, nullptr, cmd.m_primCount);
	Unbind();
}

VAO::IndexedDraw::IndexedDraw(const VertexParams& v, const ElementParams& e, const IndexedDrawCommand& cmd)
	: m_vao(IndexedVAO(v, e)), m_cmd(cmd)
{
	m_cmd.m_offset = 0;
}

void VAO::IndexedDraw::Draw(GLsizei primCount) const
{
	m_vao.Draw({ m_cmd.m_mode, m_cmd.m_count, m_cmd.m_offset, primCount });
}

VBO::ElementArrayBuffer::ElementArrayBuffer(const void* data, unsigned int size, GLenum type, GLenum usage)
	: AbstractVBO(data, size, GL_ELEMENT_ARRAY_BUFFER, usage)
	, m_type(type)
{
	
}

void VBO::ElementArrayBuffer::Draw(const IndexedDrawCommand& cmd) const
{
	Bind();
	glDrawElements(cmd.m_mode, cmd.m_count, m_type, nullptr);
	Unbind();
}