#version 330 core

struct Material {
    sampler2D diffuse;
    vec3 diffuseColor;
    sampler2D specular;
    vec3 specularColor;
    vec3 color;
    float shininess;
}; 

struct PointLight {    
    vec3 position;
    float range;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define NR_POINT_LIGHTS 4
uniform PointLight _pointLights[NR_POINT_LIGHTS];
uniform DirLight _dirLight;

uniform Material _material;
uniform vec3 _cameraPos;

in vec3 color;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  

void main ()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(_cameraPos - fragPos);
	
	vec3 result = CalcDirLight(_dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(_pointLights[i], norm, fragPos, viewDir);  

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
    // combine results
    vec3 ambient  = _material.color         * _material.diffuseColor  * light.ambient  *        vec3(texture(_material.diffuse, texCoords));
    vec3 diffuse  = _material.diffuseColor                            * light.diffuse  * diff * vec3(texture(_material.diffuse, texCoords));
    vec3 specular = _material.specularColor                           * light.specular * spec * vec3(texture(_material.specular, texCoords));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float linear = 0.22 / light.range * 20;
    float quadratic = (0.2 / light.range * 20) / (light.range * 20);
    float attenuation = 1.0 / (1 + linear * distance + 
  			     quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = _material.diffuseColor  * _material.diffuseColor  * light.ambient  *        vec3(texture(_material.diffuse, texCoords));
    vec3 diffuse  = _material.diffuseColor                            * light.diffuse  * diff * vec3(texture(_material.diffuse, texCoords));
    vec3 specular = _material.specularColor                           * light.specular * spec * vec3(texture(_material.specular, texCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 