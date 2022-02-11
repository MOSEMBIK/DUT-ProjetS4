#version 330 core

in vec3 color;
in vec3 normal;
in vec3 fragPos;
in float ambientStrength;
in vec3 ambientColor;
in vec3 lightPos;

void main ()
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	vec3 ambient = ambientColor * ambientStrength;
	vec3 result = (ambient + diffuse) * color;

	gl_FragColor = vec4(result, 1.0);
}