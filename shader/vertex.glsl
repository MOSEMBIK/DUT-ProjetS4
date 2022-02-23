#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 u_M, u_V, u_P;
uniform mat3 u_iTM;

out vec3 color;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main ()
{
	gl_Position = u_P * u_V * u_M * vec4 (aPos, 1.0);

	color = aColor;
	fragPos = vec3(u_M * vec4(aPos, 1.0));
    normal = u_iTM * aNormal;
	texCoords = aTexCoord;
}