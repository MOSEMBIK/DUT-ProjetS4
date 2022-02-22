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
uniform PointLight u_pointLights[NR_POINT_LIGHTS];
uniform DirLight u_dirLight;

uniform Material u_material;
uniform vec3 u_cameraPos;

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
	vec3 viewDir = normalize(u_cameraPos - fragPos);
	
	vec3 result = CalcDirLight(u_dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(u_pointLights[i], norm, fragPos, viewDir);  

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // combine results
    vec3 ambient  = u_material.color         * u_material.diffuseColor  * light.ambient  *        vec3(texture(u_material.diffuse, texCoords));
    vec3 diffuse  = u_material.diffuseColor                            * light.diffuse  * diff * vec3(texture(u_material.diffuse, texCoords));
    vec3 specular = u_material.specularColor                           * light.specular * spec * vec3(texture(u_material.specular, texCoords));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float linear = 0.22 / light.range * 20;
    float quadratic = (0.2 / light.range * 20) / (light.range * 20);
    float attenuation = 1.0 / (1 + linear * distance + 
  			     quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = u_material.diffuseColor  * u_material.diffuseColor  * light.ambient  *        vec3(texture(u_material.diffuse, texCoords));
    vec3 diffuse  = u_material.diffuseColor                            * light.diffuse  * diff * vec3(texture(u_material.diffuse, texCoords));
    vec3 specular = u_material.specularColor                           * light.specular * spec * vec3(texture(u_material.specular, texCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 