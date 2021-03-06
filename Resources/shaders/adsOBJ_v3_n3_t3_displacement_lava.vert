#version 410 core //compatibility

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;

uniform mat4 MMatrix;		//Model matrix

//The prefix wc means WORLD SPACE
out vec3 wcPosition;
out vec3 wcNormal;
out vec2 wcTexCoords;

void main()
{
	wcPosition 	= vec3(MMatrix * vec4(VertexPosition, 1));
	wcNormal 	= vec3(MMatrix * vec4(VertexNormal, 0));
	wcTexCoords = VertexTex.xy * 5;
}
