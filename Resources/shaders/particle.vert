#version 330 core

layout (location = 0) in vec3 vertexPosition; // <vec2 position, vec2 texCoords>
layout (location = 0) in vec2 uv; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 5.0;
    TexCoords = uv;
    ParticleColor = color;
    // gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);

    gl_Position = PMatrix * VMatrix * vec4(vertexPosition * scale + offset, 1);
}