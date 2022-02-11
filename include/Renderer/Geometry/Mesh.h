#pragma once

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
        Mesh(vector<Vertex> vertices);
        Mesh(vector<Vertex> vertices, vector<Texture> textures, GLenum drawType);
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, GLenum drawType);
        void Draw(Shader &shader);

        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vec3 color = vec3(1.0f))
        {
            vector<Vertex> vertices;
            for(int i = 0; i < positions.size(); i++)
            {
                vertices.push_back({positions[i], normals[i], vec2(0.0f), color});
            }
            return vertices;
        }

        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec3> colors)
        {
            vector<Vertex> vertices;
            for(int i = 0; i < positions.size(); i++)
            {
                vertices.push_back({positions[i], normals[i], vec2(0.0f), colors[i]});
            }
            return vertices;
        }
        
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vec3 color = vec3(1.0f))
        {
            vector<Vertex> vertices;
            for(int i = 0; i < positions.size(); i++)
            {
                vertices.push_back({positions[i], normals[i], texCoords[i], color});
            }
            return vertices;
        }
        
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords)
        {
            vector<Vertex> vertices;
            for(int i = 0; i < positions.size(); i++)
            {
                vertices.push_back({positions[i], normals[i], texCoords[i], vec3(1.0f)});
            }
            return vertices;
        }
        
        static vector<Vertex> CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vector<vec3> colors)
        {
            vector<Vertex> vertices;
            for(int i = 0; i < positions.size(); i++)
            {
                vertices.push_back({positions[i], normals[i], texCoords[i], colors[i]});
            }
            return vertices;
        }
    private:
        //  render data
        GLenum drawType;
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  