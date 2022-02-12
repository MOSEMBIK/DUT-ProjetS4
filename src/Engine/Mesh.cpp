#include <Engine/Mesh.h>
#include <numeric>

using namespace std;
using namespace glm;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, GLenum drawType = GL_TRIANGLES)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->drawType = drawType;

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<Texture> textures, GLenum drawType = GL_TRIANGLES)
{
    this->vertices = vertices;
    indices = vector<unsigned int>(vertices.size());
    iota(indices.begin(), indices.end(), 0);
    this->textures = textures;
    this->drawType = drawType;
    
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                &indices[0], GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // color
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colors));
    glBindVertexArray(0);
}  

void Mesh::Draw(Shader &shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;
        if(name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "specular")
            number = std::to_string(specularNr++);

        shader.SetUniformValue(("_material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(this->drawType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}  

vector<Texture> Mesh::LoadTextures(string diffuseTexture, string specularTexture)
{
    int width, height, nrChannel;
    unsigned char* dataDiffuse = stbi_load(diffuseTexture.c_str(), &width, &height, &nrChannel, 0);
    if(!dataDiffuse)
    {
        throw invalid_argument("Failed to load texture");
    }

    unsigned int textureIdDiffuse;
    glGenTextures(1, &textureIdDiffuse);

    glBindTexture(GL_TEXTURE_2D, textureIdDiffuse);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataDiffuse);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(dataDiffuse);
    
    unsigned char* dataSpecular = stbi_load(specularTexture.c_str(), &width, &height, &nrChannel, 0);
    if(!dataSpecular)
    {
        throw invalid_argument("Failed to load texture");
    }

    unsigned int textureIdSpecular;
    glGenTextures(1, &textureIdSpecular);   

    glBindTexture(GL_TEXTURE_2D, textureIdSpecular);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataSpecular);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(dataSpecular);

    return {{ textureIdDiffuse, "diffuse" }, { textureIdSpecular, "specular" }};
}

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vec3 color)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], vec2(0.0f), color});
    }
    return vertices;
}

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec3> colors)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], vec2(0.0f), colors[i]});
    }
    return vertices;
}

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vec3 color)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], texCoords[i], color});
    }
    return vertices;
}

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> texCoords, vector<vec3> colors)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], texCoords[i], colors[i]});
    }
    return vertices;
}