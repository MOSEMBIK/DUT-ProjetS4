#pragma once

#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Renderer/Shader.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Colors;
};

struct Texture {
    unsigned int id;
    std::string type;
};  

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        
        /**
         * @brief Construct a new Mesh object from vertices, texture and draw type
         * 
         * @param vertices List of Vertices (position, normal, texCoords and color of a space point)
         * @param textures List of Textures ("diffuse" or "specular") of the model
         * @param drawType Type of draw (GL_TRIANGLE, GL_TRIANGLE_STRIP, ...)
         */
        Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, GLenum drawType);
        
        /**
         * @brief Construct a new Mesh object from vertices, texture and draw type
         * 
         * @param vertices List of Vertices (position, normal, texCoords and color of a space point)
         * @param indices List of Indices (order in which vertex has to be used)
         * @param textures List of Textures ("diffuse" or "specular") of the model
         * @param drawType Type of draw (GL_TRIANGLE, GL_TRIANGLE_STRIP, ...)
         */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, GLenum drawType);
        
        /**
         * @brief Draw the mesh according to the vertices, textures, etc... using the given shader.
         * 
         * @param shader Shader used for drawing
         */
        void Draw(Shader &shader);

        /**
         * @brief Load a set of diffuse and specular texture from path for the basic shader
         * 
         * @param diffuseTexture Path to diffuse texture (texture applied)
         * @param specularTexture Path to specular texture (reflected surfaces)
         * @return std::vector<Texture> Set of texture for the basic shader
         */
        static std::vector<Texture> LoadTextures(std::string diffuseTexture, std::string specularTexture);

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with an unique color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param color Color for all vertices
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> CreateFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, glm::vec3 color = glm::vec3(1.0f));

        /**
         * @brief Create a std::vector of vertices from multiple std::vectors
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param colors List of colors
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> CreateFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors);
        
        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with texture and unique color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param texCoords List of texture coordinates
         * @param color Color for all vertices
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> CreateFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, glm::vec3 color = glm::vec3(1.0f));
        
        /**
         * @brief Create a std::vector of vertices from multiple std::vectors with texture and color
         * 
         * @param positions List of positions
         * @param normals List of normals
         * @param color List of colors
         * @return std::vector<Vertex> 
         */
        static std::vector<Vertex> CreateFromVectors(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> colors);
    private:
        //  render data
        GLenum drawType;
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  