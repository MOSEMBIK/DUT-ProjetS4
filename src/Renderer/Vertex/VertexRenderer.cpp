#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include <Renderer/Vertex/VertexRenderer.h>

using namespace Renderer::Vertex;

VBO::AbstractVBO::AbstractVBO(const void* data, unsigned int size, int target, GLenum usage) : m_target(target)
{
	glCreateBuffers(1, &m_rendererID);

	glBindBuffer(target, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(target, 0);
}

void VBO::AbstractVBO::Bind(std::function<void(const AbstractVBO*)> func, bool unbind) const
{
	glBindBuffer(m_target, m_rendererID);

	func(this);

	if (unbind)
		glBindBuffer(m_target, 0);
}

void Attribs(const std::vector<VertexAttri>& attribs, unsigned int divisor = 0)
{
	for (int i = 0; i < attribs.size(); ++i)
	{
		auto attrib = VertexAttri(attribs[i]);
		attrib.m_index = i;
		attrib.m_normalized = false;
		attrib.m_stride = 0;
		attrib.m_offset = 0;
		attrib.m_divisor = divisor;
		glEnableVertexAttribArray(attrib.m_index);
		glVertexAttribPointer(attrib.m_index, attrib.m_size, attrib.m_type, attrib.m_normalized, attrib.m_stride, (GLvoid*)(attrib.m_offset * sizeof(GLfloat)));
		glVertexAttribDivisor(attrib.m_index, attrib.m_divisor);
	}
}

VBO::ArrayBuffer::ArrayBuffer(const void* data, unsigned int size, const std::vector<VertexAttri>& attribs, GLenum usage)
	: m_attribs(attribs), AbstractVBO(data, size, GL_ARRAY_BUFFER, usage)
{

}

void VBO::ArrayBuffer::Bind(std::function<void(const ArrayBuffer*)> func, bool unbind) const 
{
	AbstractVBO::Bind([func, this](const AbstractVBO* vbo) {
		Attribs(this->m_attribs);
		func(this);
		}, unbind);
}

void VBO::ArrayBuffer::Draw(const DirectDrawCommand& cmd) const
{
	this->Bind([cmd](const AbstractVBO* vbo) {
		glDrawArrays(cmd.m_mode, cmd.m_first, cmd.m_count);
		});
}

VAO::AbstractVAO::AbstractVAO()
{
	glCreateVertexArrays(1, &m_vaoId);
}

void VAO::AbstractVAO::Bind(std::function<void(const AbstractVAO*)> func) const
{
	glBindVertexArray(m_vaoId);
	func(this);
	glBindVertexArray(0);
}

VAO::DirectVAO::DirectVAO(const VertexParams& v) 
	: m_vertices(VBO::ArrayBuffer(&v.m_data[0], v.m_data.size() * sizeof(GLfloat), v.m_attribs, v.m_usage))
{
	VAO::AbstractVAO::Bind([this](const AbstractVAO* vao) {
		this->m_vertices.Bind([](const VBO::ArrayBuffer*) {}, false);
		});
}

void VAO::DirectVAO::Draw(const DirectDrawCommand& cmd) const
{
	VAO::AbstractVAO::Bind([cmd](const AbstractVAO* vao) {
		glDrawArraysInstanced(cmd.m_mode, cmd.m_first, cmd.m_count, cmd.m_primCount);
		});
}

VAO::DirectDraw::DirectDraw(const VertexParams& v, const DirectDrawCommand& cmd) : m_vao(DirectVAO(v)), m_cmd(cmd)
{
	this->m_cmd.m_first = 0;
}

void VAO::DirectDraw::Draw(GLsizei primCount) const
{
	this->m_vao.Draw({ m_cmd.m_mode, m_cmd.m_first, m_cmd.m_count, primCount });
}
