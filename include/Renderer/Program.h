#pragma once

#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include <map>

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
        std::vector<std::string> m_attribs;
        std::map<char*, char*, cmp_str> m_uniforms;
        std::string m_vertexShader;
        std::string m_fragmentShader;
    };

    class Program {
    private:
        GLuint m_program;
        std::map<char*, void (*)(void*, GLuint, const char*), cmp_str> m_uniforms;
    public:
        Program(const ProgramParams&);
        void Use(const std::map<char*, void*, cmp_str>&, std::function<void(const Program*)>) const;
    };
}
