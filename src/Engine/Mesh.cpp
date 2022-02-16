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

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, GLenum drawType)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_uvs));
    // color
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_colors));
    glBindVertexArray(0);
}  

void Mesh::Draw() const
{
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

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> uvs, vec3 color)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], uvs[i], color});
    }
    return vertices;
}

vector<Vertex> Mesh::CreateFromVectors(vector<vec3> positions, vector<vec3> normals, vector<vec2> uvs, vector<vec3> colors)
{
    vector<Vertex> vertices;
    for(size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({positions[i], normals[i], uvs[i], colors[i]});
    }
    return vertices;
}
