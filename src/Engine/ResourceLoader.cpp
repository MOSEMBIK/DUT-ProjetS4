#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Engine/ResourceLoader.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <glm/geometric.hpp>

using namespace Resource;
using namespace std;
using namespace glm;

bool Resource::loadOBJ(const char* filename, vector<Mesh*>& meshes, vector<Material>& materials)
{
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        cout << "Impossible to open the file '" << filename << " !" << endl;
        return false;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    int meshCount = 0;
    while( 1 )
    {

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
            
        if (strcmp(lineHeader, "usemtl") == 0)
        {
            if(vertexIndices.size() > 0)
            {
                std::vector<glm::vec3> vertices;
                std::vector<glm::vec2> uvs;
                std::vector<glm::vec3> normals;
                for( unsigned int i=0; i<vertexIndices.size(); i++ )
                {
                    unsigned int vertexIndex = vertexIndices[i];
                    vec3 vertex = temp_vertices[vertexIndex - 1];
                    vertices.push_back(vertex);
                }

                for( unsigned int i=0; i<uvIndices.size(); i++ )
                {
                    unsigned int uvIndex = uvIndices[i];
                    vec2 uv = temp_uvs[uvIndex - 1];
                    uvs.push_back(uv);
                }

                for( unsigned int i=0; i<normalIndices.size(); i++ )
                {
                    unsigned int normalIndex = normalIndices[i];
                    vec3 normal = temp_normals[normalIndex - 1];
                    normals.push_back(normal);
                }
                vertexIndices.clear();
                uvIndices.clear();
                normalIndices.clear();

                Mesh* mesh = new Mesh(Mesh::createFromVectors(vertices, normals, uvs));
                Mesh::save(string(filename) + "_" + to_string(meshCount++), mesh);
                meshes.push_back(mesh);
            }
        }
        else if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "mtllib") == 0)
        {
            char materialName[128];
            fscanf(file, "%s\n", materialName );
            
            string filenameS = string(filename);
            unsigned int lastSlash = filenameS.find_last_of('/');
        
            Resource::loadMTL((filenameS.substr(0, lastSlash + 1) + materialName).c_str(), materials);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                cout << "File can't be read by the parser";
                fclose(file);
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    fclose(file);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {
        unsigned int vertexIndex = vertexIndices[i];
        vec3 vertex = temp_vertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }

    for( unsigned int i=0; i<uvIndices.size(); i++ )
    {
        unsigned int uvIndex = uvIndices[i];
        vec2 uv = temp_uvs[uvIndex - 1];
        uvs.push_back(uv);
    }

    for( unsigned int i=0; i<normalIndices.size(); i++ )
    {
        unsigned int normalIndex = normalIndices[i];
        vec3 normal = temp_normals[normalIndex - 1];
        normals.push_back(normal);
    }

    Mesh* mesh = new Mesh(Mesh::createFromVectors(vertices, normals, uvs));
    Mesh::save(string(filename) + "_" + to_string(meshCount), mesh);
    meshes.push_back(mesh);

    return true;
}

bool Resource::loadMTL(const char* filename, vector<Material>& materials)
{
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        cout << "Impossible to open the file '" << filename << " !" << endl;
        return false;
    }

    while( 1 )
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if (strcmp(lineHeader, "newmtl") == 0)
        {
            char materialName[128]; 
            fscanf(file, "%s\n", materialName);
            materials.push_back(Material());
        }
        else if (strcmp(lineHeader, "Ka") == 0)
        {
            glm::vec3 ambientColor;
            fscanf(file, "%f %f %f\n", &ambientColor.x, &ambientColor.y, &ambientColor.z );
            materials.back().SetAmbientColor(ambientColor);
        }
        else if (strcmp(lineHeader, "Kd") == 0)
        {
            glm::vec3 diffuseColor;
            fscanf(file, "%f %f %f\n", &diffuseColor.x, &diffuseColor.y, &diffuseColor.z );
            materials.back().setDiffuseColor(diffuseColor);
        }
        else if (strcmp(lineHeader, "Ks") == 0)
        {
            glm::vec3 specularColor;
            fscanf(file, "%f %f %f\n", &specularColor.x, &specularColor.y, &specularColor.z );
            materials.back().setSpecularColor(specularColor);
        }
        else if (strcmp(lineHeader, "Ns") == 0)
        {
            float specularExponent;
            fscanf(file, "%f\n", &specularExponent);
            materials.back().setSpecularExponent(specularExponent);
        }
        else if (strcmp(lineHeader, "map_Kd") == 0)
        {
            char diffuseMap[128]; 
            fscanf(file, "%s\n", diffuseMap);
            Texture texture;
            Resource::loadTexture(diffuseMap, texture);
            materials.back().setDiffuseTexture(texture.m_id);
        }
        else if (strcmp(lineHeader, "map_Ks") == 0)
        {
            char specularMap[128];
            fscanf(file, "%s\n", specularMap);
            Texture texture;
            Resource::loadTexture(specularMap, texture);
            materials.back().setSpecularTexture(texture.m_id);
        }
    }

    fclose(file);
    return true;
}

bool Resource::loadTexture(const char* filename, Texture& texture)
{
    int nrChannel;
    unsigned char* data = stbi_load(filename, &texture.m_width, &texture.m_height, &nrChannel, 0);
    if(!data)
    {
        cout << "Failed to load texture" << endl;
        return false;
    }

    glGenTextures(1, &texture.m_id);
    glBindTexture(GL_TEXTURE_2D, texture.m_id);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.m_width, texture.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return true;
}
