#include <Engine/Material.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace glm;
using namespace std;

Material::Material() : 
    m_ambientColor(vec3(1.0f)),
    m_diffuseColor(vec3(1.0f)),
    m_diffuseTexture(1),
    m_specularColor(vec3(1.0f)),
    m_specularTexture(1),
    m_specularExponent(32.0f),
    m_shader(Shader::find("Base"))
{
    
}

Material::Material(const Shader& shader) : 
    m_ambientColor(vec3(1.0f)),
    m_diffuseColor(vec3(1.0f)),
    m_diffuseTexture(1),
    m_specularColor(vec3(1.0f)),
    m_specularTexture(1),
    m_specularExponent(32.0f),
    m_shader(&shader)
{
    
}

void Material::use() const
{
    if(m_shader == nullptr)
    {
        cout << "Shader undefined" << endl;
        return;
    }

    m_shader->use();

    m_shader->setUniformValue("u_material.color", m_ambientColor);
    m_shader->setUniformValue("u_material.diffuseColor", m_diffuseColor);
    m_shader->setUniformValue("u_material.specularColor", m_specularColor);
    m_shader->setUniformValue("u_material.shininess", m_specularExponent);

    glActiveTexture(GL_TEXTURE0);
    m_shader->setUniformValue("u_material.diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);  

    glActiveTexture(GL_TEXTURE0 + 1);
    m_shader->setUniformValue("u_material.specular", 1);
    glBindTexture(GL_TEXTURE_2D, m_specularTexture);
    
    glActiveTexture(GL_TEXTURE0);
}