#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light _light;  

uniform Material _material;
uniform vec3 _cameraPos;

in vec3 color;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 FragColor;

void main ()
{
	// Ambiant light
	vec3 ambient = _light.ambient * vec3(texture(_material.diffuse, texCoords)) * _material.diffuse_color;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(_light.position - fragPos);
	
	// Diffuse light
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * _light.diffuse * vec3(texture(_material.diffuse, texCoords)) * _material.diffuse_color;

	// Specular reflexion
	vec3 viewDir = normalize(_cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
	vec3 specular = (vec3(texture(_material.specular, texCoords)) * spec) * _light.specular  * _material.specular_color;  

	vec3 result = (ambient + diffuse + specular) * color;

	FragColor = vec4(result, 1.0);
}