#version 330 core

uniform sampler2D u_imageTexture;
uniform vec2 u_dimensions;
uniform vec2 u_border;
uniform vec3 u_color;

in vec2 texCoords;

out vec4 FragColor;

float map(float value, float originalMin, float originalMax, float newMin, float newMax) {
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
} 

float processAxis(float coord, float textureBorder, float windowBorder) {
    if (coord < windowBorder)
        return map(coord, 0, windowBorder, 0, textureBorder) ;
    if (coord < 1 - windowBorder) 
        return map(coord,  windowBorder, 1 - windowBorder, textureBorder, 1 - textureBorder);
    return map(coord, 1 - windowBorder, 1, 1 - textureBorder, 1);
} 

void main ()
{
    vec2 newUV = vec2(
        processAxis(texCoords.x, u_border.x, u_dimensions.x),
        processAxis(texCoords.y, u_border.y, u_dimensions.y)
    );

    FragColor = texture(u_imageTexture, newUV) * vec4(u_color, 1.0);
}
