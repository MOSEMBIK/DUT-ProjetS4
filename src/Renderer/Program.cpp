#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Renderer/Program.h>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace Renderer;


GLuint Shader(GLenum type, const GLchar* source)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &source, NULL);
    glCompileShader(s);
    return s;
}

GLint Location(GLuint p, const char* name)
{
    return glGetUniformLocation(p, name);
}

void UniMat4(void* x, GLuint p, const char* name)
{
    GLint l = Location(p, name);
    glUniformMatrix4fv(l, 1, GL_FALSE, (GLfloat *)x);
}

Program::Program(const ProgramParams& params)
{
    GLuint p = glCreateProgram();

    glAttachShader(p, Shader(GL_VERTEX_SHADER, &params.m_vertexShader[0]));
    glAttachShader(p, Shader(GL_FRAGMENT_SHADER, &params.m_fragmentShader[0]));

    for(unsigned long i = 0; i < params.m_attribs.size(); ++i)
        glBindAttribLocation(p, i, &params.m_attribs[i][0]);

    glLinkProgram(p);

    for(auto uni : params.m_uniforms)
    {
        /*if(strcmp(uni.second,"int") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniform1i(l, *(GLint *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"float") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniform1f(l, *(GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"vec2") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniform2fv(l, 1, (GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"vec3") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniform3fv(l, 1, (GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"vec4") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniform4fv(l, 1, (GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"mat2") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniformMatrix2fv(l, 1, GL_FALSE, (GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else if(strcmp(uni.second,"mat3") == 0)
        {
            auto func = [p, uni](void* x){
                GLint l = Location(p, uni.first);
                glUniformMatrix3fv(l, 1, GL_FALSE, (GLfloat *)x);
            };
            m_uniforms[uni.first] = &func;
        }
        else*/ if(strcmp(uni.second,"mat4") == 0)
        {
            m_uniforms[uni.first] = &UniMat4;
        }
    }

    m_program = p;
}

void Program::Use(const std::map<char*, void*, cmp_str>& uniforms, std::function<void(const Program*)> func) const
{
    glUseProgram(m_program);

    for(auto uni : uniforms)
    {
        auto m = m_uniforms.find(uni.first);
        if(m != m_uniforms.end())
        {
            ((void(*)(void*, GLuint, const char*))m->second)(uni.second, m_program, uni.first);
        }
        
    }

    func(this);

    glUseProgram(0);
}