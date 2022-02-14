#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Resource {
    /**
     * @brief Load a .obj file and fill the vectors givens
     * 
     * @param filename Path of the file
     * @param vertices Vector of positions to fill
     * @param texCoords Vector of texture coordinates to fill
     * @param normals Vector of normals to fill
     * @return true Loaded with success
     * @return false Failed to load
     */
    bool LoadOBJ(const char* filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals);

    /**
     * @brief Load texture file and give the textureId;
     * 
     * @param filename Path of the file
     * @param textureID ID of the texture
     * @return true Loaded with success
     * @return false Failed to load
     */
    bool LoadTexture(const char* filename, unsigned int& textureID);
}