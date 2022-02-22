#include <Engine/Primitives.hpp>
#include <Engine/ResourceLoader.hpp>

using namespace Primitives;
using namespace std;
using namespace glm;

Mesh* Primitives::FastCube()
{
    Mesh* fastCube = Mesh::Find("Primitives/FastCube");
    if(fastCube == nullptr)
    {
        vector<vec3> positions = { vec3(-1, -1, -1), vec3(+1, -1, -1), vec3(-1, +1, -1), vec3(+1, +1, -1), vec3(-1, -1, +1), vec3(+1, -1, +1), vec3(-1, +1, +1), vec3(+1, +1, +1) };
        vector<unsigned int> indices = { 3, 2, 7, 6, 4, 2, 0, 3, 1, 7, 5, 4, 1, 0};

        vector<Vertex> vertices = Mesh::CreateFromVectors(positions, positions, vec3(1.0f));

        fastCube = new Mesh(vertices, indices, GL_TRIANGLE_STRIP);
        Mesh::Register("Primitives/FastCube", fastCube);
    }
    return fastCube;
}

Mesh* Primitives::Cube()
{
    Mesh* cube = Mesh::Find("Primitives/Cube");
    if(cube == nullptr)
    {
        vector<vec3> positions;
        vector<vec3> normals;
        vector<vec2> texCoords;
        vector<unsigned int> indices;
        for (int f = 0; f < 6; f++)
        {
            for (int v = 0; v < 4; v++)
            {
                positions.push_back(VERTICES [FACES [f][v]]);
                normals.push_back(NORMALS [f]);
                texCoords.push_back(TEXTURE [v]);
            }
            for (int j = 0; j < 6; j++)
            {
                indices.push_back(FACE_INDEX_ORDER[j] + 4 * f);
            }
        }

        vector<Vertex> vertices = Mesh::CreateFromVectors(positions, normals, texCoords, vec3(1.0f, 1.0f, 1.0f));

        cube = new Mesh(vertices, indices, GL_TRIANGLES);
        Mesh::Register("Primitives/Cube", cube);
    }
    return cube;
}

Mesh* Primitives::Quad()
{
    Mesh* quad = Mesh::Find("Primitives/Quad");
    if(quad == nullptr)
    {
        vector<vec3> positions = { vec3(-0.5, -0.5, 0), vec3(0.5, -0.5, 0), vec3(-0.5, 0.5, 0), vec3(0.5, 0.5, 0) };
        vector<vec2> uvs = { vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) };
        
        vector<Vertex> vertices = Mesh::CreateFromVectors(positions, positions, uvs, vec3(1.0f));

        quad = new Mesh(vertices, GL_TRIANGLE_STRIP);
        Mesh::Register("Primitives/Quad", quad);
    }
    return quad;
}
