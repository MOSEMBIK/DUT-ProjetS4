#pragma once

#include <vector>
#include <Renderer/Geometry/Mesh.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;
using namespace std;

namespace Primitives
{
    vector<vec3> VERTICES =
    {
        vec3(-1, -1, -1),
        vec3(+1, -1, -1),
        vec3(-1, +1, -1),
        vec3(+1, +1, -1),
        vec3(-1, -1, +1),
        vec3(+1, -1, +1),
        vec3(-1, +1, +1),
        vec3(+1, +1, +1) 
    };
    
    vector<ivec4> FACES =
    {
        ivec4(0, 4, 2, 6),
        ivec4(5, 1, 7, 3),
        ivec4(0, 1, 4, 5),
        ivec4(6, 7, 2, 3),
        ivec4(1, 0, 3, 2),
        ivec4(4, 5, 6, 7) 
    };

    vector<vec3> NORMALS =
    {
        vec3(-1,  0,  0),
        vec3(+1,  0,  0),
        vec3( 0, -1,  0),
        vec3( 0, +1,  0),
        vec3( 0,  0, -1),
        vec3( 0,  0, +1)
    };

    vector<unsigned int> FACE_INDEX_ORDER = { 0, 1, 2, 2, 1, 3 };

    Mesh FastCube()
    {
        vector<vec3> positions = { vec3(-1, -1, -1), vec3(+1, -1, -1), vec3(-1, +1, -1), vec3(+1, +1, -1), vec3(-1, -1, +1), vec3(+1, -1, +1), vec3(-1, +1, +1), vec3(+1, +1, +1) };
        vector<unsigned int> indices = { 3, 2, 7, 6, 4, 2, 0, 3, 1, 7, 5, 4, 1, 0};

        vector<Vertex> vertices = Mesh::CreateFromVectors(positions, positions, vec3(0.1f, 0.5f, 0.8f));

        return Mesh(vertices, indices, vector<Texture>(), GL_TRIANGLE_STRIP);
    }

    Mesh Cube()
    {
        vector<vec3> positions;
        for (int f = 0; f < 6; f++)
        {
            for (int v = 0; v < 4; v++)
            {
                positions.push_back(VERTICES [FACES [f][v]]);
            }
        }
        
        vector<vec3> normals;
        for (int f = 0; f < 6; f++)
        {
            for (int v = 0; v < 4; v++)
            {
                normals.push_back(NORMALS [f]);
            }
        }

        vector<unsigned int> indices;
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                indices.push_back(FACE_INDEX_ORDER[j] + 4 * i);
            }
        }

        vector<Vertex> vertices = Mesh::CreateFromVectors(positions, normals, vec3(0.1f, 0.5f, 0.8f));

        return Mesh(vertices, indices, vector<Texture>(), GL_TRIANGLES);
    }
}