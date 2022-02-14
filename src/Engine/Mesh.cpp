#include <Engine/Mesh.hpp>
#include <numeric>

using namespace std;
using namespace glm;

Mesh::Mesh(vector<Vertex> vertices, GLenum drawType)
{
    this->m_vertices = vertices;
    m_indices = vector<unsigned int>(vertices.size());
    iota(m_indices.begin(), m_indices.end(), 0);
    this->drawType = drawType;
    this->m_textures = vector<Texture>();

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, GLenum drawType)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_textures = vector<Texture>();
    this->drawType = drawType;

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, GLenum drawType)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_textures = textures;
    this->drawType = drawType;

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<Texture> textures, GLenum drawType)
{
    this->m_vertices = vertices;
    m_indices = vector<unsigned int>(vertices.size());
    iota(m_indices.begin(), m_indices.end(), 0);
    this->m_textures = textures;
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

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                &m_indices[0], GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    // texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));
    // color
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_colors));
    glBindVertexArray(0);
}  

void Mesh::Draw(Shader &shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = m_textures[i].m_type;
        if(name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "specular")
            number = std::to_string(specularNr++);

        shader.SetUniformValue(("_material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(this->drawType, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
