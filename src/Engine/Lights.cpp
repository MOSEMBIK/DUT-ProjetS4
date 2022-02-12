#include <Engine/Lights.hpp>

using namespace std;
using namespace glm;

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular)
{
    this->direction = direction;
}

PointLight::PointLight(int index, float distance, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular)
{
    this->index = index;
    this->range = distance;
    this->position = position;
    this->enabled = false;
}

void DirectionalLight::SendToShader(const Shader& shader)
{
	shader.SetUniformValue("_dirLight.direction", direction);
	shader.SetUniformValue("_dirLight.ambient", ambient);
	shader.SetUniformValue("_dirLight.diffuse", diffuse);
	shader.SetUniformValue("_dirLight.specular", specular);
}

void PointLight::SendToShader(const Shader& shader)
{
    string lightIndexStr = to_string(index);
	shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].position").c_str(), position);
	shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].range").c_str(), range);
    if(enabled)
    {
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].ambient").c_str(), ambient);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].diffuse").c_str(), diffuse);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].specular").c_str(), specular);
    }
    else
    {
        vec3 noLight = vec3(0.0f);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].ambient").c_str(), noLight);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].diffuse").c_str(), noLight);
        shader.SetUniformValue(("_pointLights[" + lightIndexStr + "].specular").c_str(), noLight);
    }
}