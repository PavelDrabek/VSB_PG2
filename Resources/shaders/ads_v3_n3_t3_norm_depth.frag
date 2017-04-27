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

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 texCoord;

const float scale = 0.05;
const float numSteps = 10;
float stepx = 1.0 / numSteps;

void main()
{
	vec2 newTexCoords = texCoord;
	vec3 tsTextureNormal = texture(texNormal, newTexCoords).rgb * 2 - vec3(1);
	vec4 texColor = vec4(1,0,0,1);

	if(material.hasDiffuseTexture != 0)
	{
		// texColor = texColor * texture(texDiffuse, newTexCoords);
	}
	// if(texColor.a == 0) discard;

	FragColor = light.ambient * material.ambient * texColor;

	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);

	float lambert = 0;
	if(material.transparency > 0) {
		lambert = abs(dot(N, L));
	} else {
		lambert = max(dot(N, L), 0);
	}

	if(lambert > 0) {
		FragColor += light.diffuse * 1 * texColor * lambert;
		vec3 E = normalize(ecViewDir);
		vec3 R = normalize(-reflect(L, N));
		float specular = max(pow(dot(R, E), 40), 0);
		FragColor += light.specular * 1 * texColor * specular;
	}

	// FragColor = vec4(1,0,0,1);
	// FragColor = texture(texNormal, texCoord);
	// FragColor = vec4(ambient + diffuse + specular, 1);
}
