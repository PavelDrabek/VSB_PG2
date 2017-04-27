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
	int spotExponent;		//Specifies intensity distribution of spotlight
	float spotCutoff;		//Specifies maximum spread angle of spotlight (180 = off).
};

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform float time;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 texCoord;

const float scale = 0.05;
const float numSteps = 10;
float stepx = 1.0 / numSteps;

float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = fract(x);

	// Four corners in 2D of a tile
	float a = hash(i);
	float b = hash(i + vec2(1.0, 0.0));
	float c = hash(i + vec2(0.0, 1.0));
	float d = hash(i + vec2(1.0, 1.0));

	// Simple 2D lerp using smoothstep envelope between the values.
	// return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
	//			mix(c, d, smoothstep(0.0, 1.0, f.x)),
	//			smoothstep(0.0, 1.0, f.y)));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
	vec2 newTexCoords = texCoord;
	vec3 tsTextureNormal = texture(texNormal, newTexCoords).rgb * 2 - vec3(1);
	float glow = 2 * (1 - noise(newTexCoords.xy * 2 + vec2(time * 0.05)));
	vec4 texColor = vec4(glow, glow, glow ,1);

	if(material.hasDiffuseTexture != 0)
	{
		texColor = texture(texDiffuse, newTexCoords);
	}

	FragColor = texColor * (1 + glow);
	return;
}
