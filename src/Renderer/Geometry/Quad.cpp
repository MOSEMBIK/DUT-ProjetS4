#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Renderer/Vertex/VertexRenderer.h>
#include <Renderer/Geometry/Quad.h>

using namespace Renderer::Vertex;

Quad::Quad() 
: DirectDraw(
    {
        { -1, -1, +1, -1, -1, +1, +1, +1}, 
        { { 0, 2, GL_FLOAT} },
    },
    { GL_TRIANGLE_STRIP, 4 } ) 
{

}
