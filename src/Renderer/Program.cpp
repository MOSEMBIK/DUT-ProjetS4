#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Program.h>
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

using namespace std;
using namespace Renderer;


GLuint Shader(GLenum type, const GLchar* source)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &source, NULL);
    glCompileShader(s);
    return s;
}

Program::Program(const ProgramParams& params)
{
    GLuint p = glCreateProgram();

    glAttachShader(p, Shader(GL_VERTEX_SHADER, &params.m_vertexShader[0]));
    glAttachShader(p, Shader(GL_FRAGMENT_SHADER, &params.m_fragmentShader[0]));

    glLinkProgram(p);

    m_program = p;
}

void Program::Use(std::function<void(const Program*)> func) const
{
    glUseProgram(m_program);

    func(this);

    glUseProgram(0);
}