#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_M, u_P;

out vec2 texCoords;

void main ()
{
	gl_Position = u_P * u_M * vec4 (aPos, 1.0);

	texCoords = aTexCoord;
}
