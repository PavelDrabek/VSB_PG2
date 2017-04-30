#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;

layout(location = 0) out vec4 FragColor;
// out vec4 color;

uniform sampler2D sprite;

void main()
{
    FragColor =  (texture(sprite, TexCoords) * ParticleColor);
}  