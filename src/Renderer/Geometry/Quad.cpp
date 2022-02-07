#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Renderer/VertexRenderer.h>
#include <Renderer/Geometry/Quad.h>

using namespace VertexRenderer;

Quad::Quad() : DirectDraw
(
    VAO::VertexParams {
        .m_data = {-0.5, -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5}, 
        .m_attribs = {
            VertexAttri {.m_index = 0, .m_size = 2, .m_type = GL_FLOAT}
        },
    },
    DirectDrawCommand {
        .m_mode = GL_TRIANGLE_STRIP, 
        .m_first = 0,
        .m_count = 4
    }
) { }