#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;

layout(location = 0) out vec4 FragColor;
// out vec4 color;

uniform sampler2D sprite;

void main()
{
	vec4 texColor = texture(sprite, TexCoords);
	texColor.a = min(1, length(texColor.xyz));
    FragColor =  (texColor * ParticleColor);
}  