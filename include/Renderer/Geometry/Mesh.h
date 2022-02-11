#pragma once

#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Renderer/Shader.h>

using namespace std;
using namespace glm;

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Colors;
};

struct Texture {
    unsigned int id;
    string type;
};  

class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;
        Mesh(vector<Vertex> vertices, vector<Texture> textures, GLenum drawType);
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, GLenum drawType);
        void Draw(Shader &shader);

        static vector<Texture> LoadTextures(string diffuseTexture, string specularTexture);
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vec3 color = vec3(1.0f));
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec3> colors);
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vec3 color = vec3(1.0f));
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords);
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vector<vec3> colors);
    private:
        //  render data
        GLenum drawType;
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  