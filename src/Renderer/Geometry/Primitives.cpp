#include <Renderer/Geometry/Primitives.h>
#include <iostream>

using namespace Primitives;

Mesh Primitives::FastCube()
{
    vector<vec3> positions = { vec3(-1, -1, -1), vec3(+1, -1, -1), vec3(-1, +1, -1), vec3(+1, +1, -1), vec3(-1, -1, +1), vec3(+1, -1, +1), vec3(-1, +1, +1), vec3(+1, +1, +1) };
    vector<unsigned int> indices = { 3, 2, 7, 6, 4, 2, 0, 3, 1, 7, 5, 4, 1, 0};

    vector<Vertex> vertices = Mesh::CreateFromVectors(positions, positions, vec3(1.0f));

    return Mesh(vertices, indices, vector<Texture>(), GL_TRIANGLE_STRIP);
}

Mesh Primitives::Cube()
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
            std::cout << TEXTURE[v][0] << " " << TEXTURE[v][1] << " ";
        }
        for (int j = 0; j < 6; j++)
        {
            indices.push_back(FACE_INDEX_ORDER[j] + 4 * f);
        }
    }

    vector<Vertex> vertices = Mesh::CreateFromVectors(positions, normals, texCoords, vec3(1.0f, 1.0f, 1.0f));

    return Mesh(vertices, indices, Mesh::LoadTextures("assets/diffuse.png", "assets/specular.png"), GL_TRIANGLES);
}
