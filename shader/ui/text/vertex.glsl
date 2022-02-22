#version 330 core
layout (location = 0) in vec4 aVertex;

out vec2 texCoords;

uniform mat4 u_P;

void main()
{
    gl_Position = u_P * vec4(aVertex.xy, 1.0, 1.0);
	
    texCoords = vec2(aVertex.z, 1.0 - aVertex.w);
}  