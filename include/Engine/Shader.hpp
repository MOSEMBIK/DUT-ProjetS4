#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class Shader
{
public:
    unsigned int ID;

    /**
     * @brief Construct a new Shader object
     * 
     * @param vertexPath Path of the vertex shader file
     * @param fragmentPath Path of the fragment shader file
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Define this shader as the currently used one.
     * This need to be called before setting uniform values.
     */
    void Use();

    /**
     * @brief Set the Uniform Value for the shader
     * (If the type isn't implemented, it won't set it)
     * 
     * @tparam T Type of value
     * @param name Name of the uniform variable
     * @param value Value to set
     */
    template<typename T> void SetUniformValue(const char * name, const T& value) const 
    {
        std::cout << typeid(value).name() << " type not implemented for uniforms value!" << std::endl;
    }
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

template<>
inline void Shader::SetUniformValue<GLboolean>(const char * name, const GLboolean& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform1i(l, value);
}

template<>
inline void Shader::SetUniformValue<GLuint>(const char * name, const GLuint& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform1i(l, value);
}

template<>
inline void Shader::SetUniformValue<GLint>(const char * name, const GLint& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform1i(l, value);
}

template<>
inline void Shader::SetUniformValue<GLfloat>(const char * name, const GLfloat& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform1f(l, value);
}

template<>
inline void Shader::SetUniformValue<glm::vec2>(const char * name, const glm::vec2& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform2fv(l, 1, &value[0]);
}

template<>
inline void Shader::SetUniformValue<glm::vec3>(const char * name, const glm::vec3& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform3fv(l, 1, &value[0]);
}

template<>
inline void Shader::SetUniformValue<glm::vec4>(const char * name, const glm::vec4& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniform4fv(l, 1, &value[0]);
}

template<>
inline void Shader::SetUniformValue<glm::mat2>(const char * name, const glm::mat2& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniformMatrix2fv(l, 1, GL_FALSE, &value[0][0]);
}

template<>
inline void Shader::SetUniformValue<glm::mat3>(const char * name, const glm::mat3& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniformMatrix3fv(l, 1, GL_FALSE, &value[0][0]);
}

template<>
inline void Shader::SetUniformValue<glm::mat4>(const char * name, const glm::mat4& value) const
{
    GLint l = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(l, 1, GL_FALSE, &value[0][0]);
}
