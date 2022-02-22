#version 330 core

uniform sampler2D u_buttonTexture;
in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

void main ()
{
	vec3 result = vec3(texture(u_buttonTexture, texCoords));
	result *= color;

	FragColor = vec4(result, 1.0);
}
