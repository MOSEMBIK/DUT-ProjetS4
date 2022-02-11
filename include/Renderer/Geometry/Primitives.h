#pragma once

#include <vector>
#include <Renderer/Geometry/Mesh.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;
using namespace std;

namespace Primitives
{
    static vector<vec3> VERTICES =
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
    
    static vector<ivec4> FACES =
    {
        ivec4(0, 4, 2, 6),
        ivec4(5, 1, 7, 3),
        ivec4(0, 1, 4, 5),
        ivec4(6, 7, 2, 3),
        ivec4(1, 0, 3, 2),
        ivec4(4, 5, 6, 7) 
    };

    static vector<vec3> NORMALS =
    {
        vec3(-1,  0,  0),
        vec3(+1,  0,  0),
        vec3( 0, -1,  0),
        vec3( 0, +1,  0),
        vec3( 0,  0, -1),
        vec3( 0,  0, +1)
    };

    static vector<vec2> TEXTURE =
    {
        vec2{0, 0},
        vec2{1, 0},
        vec2{0, 1},
        vec2{1, 1}
    };


    static vector<unsigned int> FACE_INDEX_ORDER = { 0, 1, 2, 2, 1, 3 };

    Mesh FastCube();
    Mesh Cube();
}