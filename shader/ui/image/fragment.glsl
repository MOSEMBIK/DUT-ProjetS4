#version 330 core

uniform sampler2D u_imageTexture;
uniform vec3 u_color;

in vec2 texCoords;

out vec4 FragColor;

void main ()
{
    vec4 texColor = texture(u_imageTexture, vec2(texCoords.x, 1-texCoords.y));
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor * vec4(u_color, 1.0);
}
