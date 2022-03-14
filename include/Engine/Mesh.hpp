#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 m_position;
    glm::vec3 m_normal = glm::vec3(0.0f);
    glm::vec2 m_uvs = glm::vec2(0.0f);
    glm::vec3 m_colors = glm::vec3(1.0f);
};

class Mesh {
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        static std::map<std::string, Mesh*> meshDict;

    public:
        void setupMesh();

        // mesh data
        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        GLenum drawType;

        /**
         * @brief Clear pointers
         * 
         */
        static void clear();

        /**
         * @brief Save the mesh in the dictionnary
         * 
         * @param name Name of the mesh
         * @param mesh 
         */
        static void save(std::string name, Mesh* mesh);

        /**
         * @brief Find a mesh in the dictionnary
         * 
         * @param name Name of the mesh
         * @return Mesh* Pointer to the mesh, nullptr if not found
         */
        static Mesh* find(std::string name);

        Mesh() = default;

        /**
         * @brief Construct a new Mesh object from vertices and draw type
         * 
         * @param vertices List of Vertices (position, normal, uvs and color of a space point)
         * @param drawType Type of draw (GL_TRIANGLE, GL_TRIANGLE_STRIP, ...)
         */
        Mesh(std::vector<Vertex> vertices, GLenum drawType = GL_TRIANGLES);
        
        /**
         * @brief Construct a new Mesh object from vertices, indices and draw type
         * 
         * @param vertices List of Vertices (position, normal, texCoords and color of a space point)
         * @param indices List of Indices (order in which vertex has to be used)
         * @param drawType Type of draw (GL_TRIANGLE, GL_TRIANGLE_STRIP, ...)
         */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, GLenum drawType = GL_TRIANGLES);
        
        /**
         * @brief Draw the mesh according to the vertices, textures, etc...
         */
        void draw() const;

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with an unique color
         * 
         * @param positions List of positions
         * @param uvs List of texture coordinates
         * @param color Color for all vertices
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs, glm::vec3 color = glm::vec3(1.0f));

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors
         * 
         * @param positions List of positions
         * @param uvs List of texture coordinates
         * @param colors List of colors
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs, std::vector<glm::vec3> colors);

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with an unique color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param color Color for all vertices
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, glm::vec3 color = glm::vec3(1.0f));

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param colors List of colors
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors);
        
        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with texture and unique color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param uvs List of texture coordinates
         * @param color Color for all vertices
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, glm::vec3 color = glm::vec3(1.0f));
        
        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with texture and color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param uvs List of texture coordinates
         * @param colors List of colors
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> createFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, std::vector<glm::vec3> colors);
};  
