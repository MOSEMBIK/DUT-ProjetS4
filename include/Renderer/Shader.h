#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace Shader
{
    const char* ReadShader(const std::string& pathToFile)
    {
        std::string path = "shader\\";
        path += pathToFile + ".glsl";
        std::string content;
        std::ifstream fileStream(path.c_str(), std::ios::in);

        if(!fileStream.is_open())
        {
            std::cerr << "Could not read file " << path << ". File does not exist.";
            return "";
        }

        std::string line = "";
        while(!fileStream.eof())
        {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content.c_str();
    }
}