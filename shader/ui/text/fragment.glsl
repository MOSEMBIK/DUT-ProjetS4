#version 330 core

uniform sampler2D u_text;
uniform vec3 u_textColor;

in vec2 texCoords;

out vec4 FragColor;

void main ()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, texCoords).r);
    FragColor = vec4(u_textColor, 1.0) * sampled;
}
