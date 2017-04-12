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

struct VS_OUT {
	vec3 tsLightDir;
	vec3 tsNormal;
	vec3 tsViewDir;
	vec2 texCoord;
};


layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTexCoord;
layout(location = 3) in vec3 T;
layout(location = 4) in vec3 B;
layout(location = 5) in vec3 N;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MMatrix;		//Model matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat4 TMatrix;		//TMatrix 

uniform LightInfo light;
uniform MaterialInfo material;

out VS_OUT vs_out;

void main()
{

	vs_out.texCoord = VertexTexCoord.xy;

	vec4 ecPosition = MVMatrix * vec4(VertexPosition, 1);
	vec3 ecLightDir = vec3((VMatrix * light.position) - ecPosition); // nevadi ze zahodime w
	vec3 ecNormal = NormalMatrix * VertexNormal;
	vec3 ecViewDir = vec3(-ecPosition); // kamera je v 0 (protoze jsme v eye coordinates)

	// vec3 T1 = NormalMatrix * T;
	// vec3 B1 = NormalMatrix * B;
	// vec3 N1 = NormalMatrix * N;

	vec3 T1 = (MVMatrix * vec4(T, 0)).xyz;
	vec3 B1 = (MVMatrix * vec4(B, 0)).xyz;
	vec3 N1 = (MVMatrix * vec4(N, 0)).xyz;

	vs_out.tsLightDir = vec3(dot(T1, ecLightDir), dot(B1, ecLightDir), dot(N1, ecLightDir));
	vs_out.tsViewDir = vec3(dot(T1, ecViewDir), dot(B1, ecViewDir), dot(N1, ecViewDir));
	vs_out.tsNormal = vec3(dot(T1, ecNormal), dot(B1, ecNormal), dot(N1, ecNormal));

	// vs_out.texCoord = vec2(TMatrix * MMatrix * vec4(VertexPosition, 1));
	gl_Position = PMatrix * ecPosition;
}
