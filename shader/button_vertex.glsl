#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 _M, _P;

out vec3 color;
out vec2 texCoords;

void main ()
{
	gl_Position = vec4 (aPos, 1.0);

	color = aColor;
	texCoords = aTexCoord;
}
