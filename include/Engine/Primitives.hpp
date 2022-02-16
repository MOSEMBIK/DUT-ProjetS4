#pragma once

#include <Engine/Mesh.hpp>

namespace Primitives
{
    static std::vector<glm::vec3> VERTICES =
    {
        glm::vec3(-1, -1, -1),
        glm::vec3(+1, -1, -1),
        glm::vec3(-1, +1, -1),
        glm::vec3(+1, +1, -1),
        glm::vec3(-1, -1, +1),
        glm::vec3(+1, -1, +1),
        glm::vec3(-1, +1, +1),
        glm::vec3(+1, +1, +1) 
    };
    
    static std::vector<std::vector<unsigned int>> FACES =
    {
        {0, 4, 2, 6},
        {5, 1, 7, 3},
        {0, 1, 4, 5},
        {6, 7, 2, 3},
        {1, 0, 3, 2},
        {4, 5, 6, 7} 
    };

    static std::vector<glm::vec3> NORMALS =
    {
        glm::vec3(-1,  0,  0),
        glm::vec3(+1,  0,  0),
        glm::vec3( 0, -1,  0),
        glm::vec3( 0, +1,  0),
        glm::vec3( 0,  0, -1),
        glm::vec3( 0,  0, +1)
    };

    static std::vector<glm::vec2> TEXTURE =
    {
        glm::vec2{0, 0},
        glm::vec2{1, 0},
        glm::vec2{0, 1},
        glm::vec2{1, 1}
    };

    static std::vector<unsigned int> FACE_INDEX_ORDER = { 0, 1, 2, 2, 1, 3 };

    /**
     * @brief Generate a cube with shared vertices (light isn't sharped)
     * 
     * @return Mesh 
     */
    Mesh FastCube();

    /**
     * @brief Generate a cube with sharped edges
     * 
     * @return Mesh 
     */
    Mesh Cube();
}
