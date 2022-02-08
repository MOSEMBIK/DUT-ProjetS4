#pragma once

#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

namespace Renderer {
    struct ProgramParams
    {
        std::string m_vertexShader;
        std::string m_fragmentShader;
    };

    class Program {
    private:
        GLuint m_program;
    public:
        Program(const ProgramParams&);
        void Use(std::function<void(const Program*)>) const;
        
        template<typename T> void SetUniformValue(const char * name, const T& value) const {}
    };

    template<>
    inline void Program::SetUniformValue<GLint>(const char * name, const GLint& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniform1i(l, value);
    }

    template<>
    inline void Program::SetUniformValue<GLfloat>(const char * name, const GLfloat& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniform1f(l, value);
    }

    template<>
    inline void Program::SetUniformValue<glm::vec2>(const char * name, const glm::vec2& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniform2fv(l, 1, &value[0]);
    }

    template<>
    inline void Program::SetUniformValue<glm::vec3>(const char * name, const glm::vec3& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniform3fv(l, 1, &value[0]);
    }

    template<>
    inline void Program::SetUniformValue<glm::vec4>(const char * name, const glm::vec4& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniform4fv(l, 1, &value[0]);
    }

    template<>
    inline void Program::SetUniformValue<glm::mat2>(const char * name, const glm::mat2& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniformMatrix2fv(l, 1, GL_FALSE, &value[0][0]);
    }

    template<>
    inline void Program::SetUniformValue<glm::mat3>(const char * name, const glm::mat3& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniformMatrix3fv(l, 1, GL_FALSE, &value[0][0]);
    }

    template<>
    inline void Program::SetUniformValue<glm::mat4>(const char * name, const glm::mat4& value) const
    {
        GLint l = glGetUniformLocation(m_program, name);
        glUniformMatrix4fv(l, 1, GL_FALSE, &value[0][0]);
    }
}