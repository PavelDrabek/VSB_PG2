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
uniform sampler2D tex1;
uniform sampler2D tex2;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecNormal;
in vec2 texCoord;
in float dotAngle;

void main()
{
	// vec3 N = normalize(ecNormal);
	// vec3 L = normalize(ecLightDir);
	// // vec3 R = normalize(-reflect(L, N));
	// vec3 R = normalize(2 * dot(L, N) * N - L);
	// vec3 ecViewDir_norm = normalize(ecViewDir);

	// float lambert = dot(R, N);
	// float phong = max(pow(dot(ecViewDir_norm, R), 40), 0);
	
	// vec3 ambient = material.ambient.xyz * light.ambient.xyz;
	// vec3 diffuse = lambert * material.diffuse.xyz * light.diffuse.xyz;
	// vec3 specular = phong * material.specular.xyz * light.specular.xyz;

	// float angle = 10;
	// float t = dot(N, vec3(0, 0, 1));

	float R = 0.80;
	float L = 0.95;
	float t;
	if(dotAngle < R) {
		t = 0;
	} else if(dotAngle > L) {
		t = 1;
	} else {
		t = 1 - (L - dotAngle) / (L - R);
	}

	vec4 texColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), t);

	FragColor = vec4(ecNormal, 1);
	FragColor = texColor;
	// if(dotAngle > L) {
	// 	FragColor = vec4(dotAngle, 0, 0, 1);
	// 	FragColor = vec4(t, 0, 0, 1);
	// }
	// FragColor = vec4(ambient + diffuse + specular, 1);
}
