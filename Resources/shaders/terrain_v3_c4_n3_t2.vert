#version 410 core //compatibility

struct MaterialInfo {
	vec4 ambient;			//Material ambient color
	vec4 diffuse;			//Material diffuse color
	vec4 specular;			//Material specular color
	float transparency;		//Material transparency
	float shininess;		//Material shininess
	int illumination;		//Material
	int hasDiffuseTexture;	//Material
};

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlighit
	float spotCutoff;			//Specifies maximum spread angle of spotlight (180 = off).
};

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MMatrix;		//Model matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat4 TMatrix;		//TMatrix 

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out vec4 ecPosition;			
out vec3 ecNormal;
out vec2 texCoord;
out float dotAngle;

void main()
{
	ecPosition = MVMatrix * vec4(VertexPosition, 1);
	ecNormal = normalize(VertexNormal);

	dotAngle = max(dot(normalize(VertexNormal), vec3(0,0,1)), 0);

	texCoord = vec2(TMatrix * MMatrix * vec4(VertexPosition, 1));
	//texCoord = VertexTexCoord;
	gl_Position = PMatrix * ecPosition;
}
