#pragma once

#include <Engine/Mesh.hpp>
#include <Engine/Material.hpp>
#include <Engine/Texture.hpp>

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
     * @param materials Vector of materials to fill
     * @return true Loaded with success
     * @return false Failed to load
     */
    bool LoadOBJ(const char* filename, std::vector<Mesh*>& meshes, std::vector<Material>& materials);

    /**
     * @brief Load texture file and give the textureId;
     * 
     * @param filename Path of the file
     * @param texture Texture information
     * @return true Loaded with success
     * @return false Failed to load
     */
    bool LoadTexture(const char* filename, Texture& texture);

    /**
     * @brief Load an MTL (Material Template Library) file into materials
     * 
     * @param filename Path of the file
     * @param material Vector to fill of material
     * @return true Loaded with sucess
     * @return false Failed to load
     */
    
    bool LoadMTL(const char* filename, std::vector<Material>& material);
}