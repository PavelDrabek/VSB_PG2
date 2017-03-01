#version 400 core //compatibility

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
	int spotExponent;		//Specifies intensity distribution of spotlight
	float spotCutoff;		//Specifies maximum spread angle of spotlight (180 = off).
};

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;

void main()
{
	vec3 V = normalize(ecViewDir);
	vec3 L = normalize(ecLightDir);
	vec3 N = normalize(ecNormal);
	vec3 R = normalize(2 * dot(L, N) * N - L);
	// vec3 R = normalize(-reflect(L, N));

	float gooch = dot(V, N);
	vec3 cFrom = vec3(1,0,0);
	vec3 cTo = vec3(0,0,1);

	vec3 c = mix(cFrom, cTo, gooch);
	if(abs(gooch) < 0.3) {
		c = vec3(0);
	}

	FragColor = vec4(c, 1);
}
