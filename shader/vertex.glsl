#version 330 core
layout (location = 0) in vec3 aPos;

uniform float time;
uniform mat4 MVP;
out vec4 color;

void main ()
{
	gl_Position = MVP * vec4 (aPos, 1.0);
	color = vec4 (0.5 - aPos, 1.0);
}