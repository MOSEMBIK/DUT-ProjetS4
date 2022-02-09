#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Renderer/Vertex/VertexRenderer.h>
#include <Renderer/Geometry/Cube.h>

using namespace Renderer::Vertex;

Cube::Cube() 
: IndexedDraw(
    {
        { -1, -1, -1, +1, -1, -1, -1, +1, -1, +1, +1, -1, -1, -1, +1, +1, -1, +1, -1, +1, +1, +1, +1, +1}, 
        { { 0, 3, GL_FLOAT} },
    },
    {
        { 3, 2, 7, 6, 4, 2, 0, 3, 1, 7, 5, 4, 1, 0}, 
        GL_UNSIGNED_INT
    },
    { GL_TRIANGLE_STRIP,  14} ) 
{

}
