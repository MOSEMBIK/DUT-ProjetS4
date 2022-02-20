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
    m_shader(Shader::Find("Base"))
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

void Material::Use() const
{
    //cout << "===" << endl;
    //cout << m_ambientColor.x << " " << m_ambientColor.y << " " << m_ambientColor.z << endl;
    //cout << m_diffuseColor.x << " " << m_diffuseColor.y << " " << m_diffuseColor.z << endl;
    //cout << m_specularColor.x << " " << m_specularColor.y << " " << m_specularColor.z << endl;
    //cout << m_specularExponent << endl;
    //cout << m_diffuseTexture << " " << m_specularTexture << endl;

    if(m_shader == nullptr)
    {
        cout << "Shader undefined" << endl;
        return;
    }

    m_shader->Use();

    m_shader->SetUniformValue("_material.color", m_ambientColor);
    m_shader->SetUniformValue("_material.diffuseColor", m_diffuseColor);
    m_shader->SetUniformValue("_material.specularColor", m_specularColor);
    m_shader->SetUniformValue("_material.shininess", m_specularExponent);

    glActiveTexture(GL_TEXTURE0);
    m_shader->SetUniformValue("_material.diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);  

    glActiveTexture(GL_TEXTURE0 + 1);
    m_shader->SetUniformValue("_material.specular", 1);
    glBindTexture(GL_TEXTURE_2D, m_specularTexture);
    
    glActiveTexture(GL_TEXTURE0);
}