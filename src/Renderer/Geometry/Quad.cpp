#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Renderer/Vertex/VertexRenderer.h>
#include <Renderer/Geometry/Quad.h>

using namespace Renderer::Vertex;

Quad::Quad() : DirectDraw
(
    VAO::VertexParams {
        {-0.5, -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5}, 
        {
            VertexAttri {0, 2, GL_FLOAT}
        },
    },
    DirectDrawCommand { GL_TRIANGLE_STRIP, 0, 4 }
) { }