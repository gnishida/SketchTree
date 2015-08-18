#version 330

// varying variables
in vec3 fColor;
in vec3 fTexCoord;
in vec3 fNormal;
in vec3 fPosition;
noperspective in vec3 dist;

// output color
out vec4 outputF;

// uniform variables
uniform int textureEnabled;	// 0 -- color / 1 -- texture
uniform sampler2D tex0;
uniform int wireframeEnalbed; // 0 -- no wireframe / 1 -- add wireframe

uniform int shadowState;	// 1 -- normal / 2 -- shadow
uniform mat4 light_mvpMatrix;
uniform vec3 lightDir;
uniform sampler2D shadowMap;

float shadowCoef(){
	vec4 shadow_coord2 = light_mvpMatrix * vec4(fPosition, 1.0);
	vec3 ProjCoords = shadow_coord2.xyz / shadow_coord2.w;
	vec2 UVCoords;
	UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
	
	float visibility = 1.0f;
	if (texture2D(shadowMap, UVCoords).z  <  z) {
		visibility = 0;
	}
	return visibility;
}

void main()
{
	// for color mode
	outputF = vec4(fColor, 1.0);

	// determine frag distance to closest edge
	float nearD = min(min(dist[0],dist[1]),dist[2]);
	float edgeIntensity = exp2(-1.0*nearD*nearD);

	// shadow
	if (shadowState == 2) {
		return;
	}

	if (textureEnabled == 1) { // for texture mode
		outputF = texture(tex0, fTexCoord.rg);
	}

	// lighting
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 diffuse = vec4(0.8, 0.8, 0.8, 1.0) * max(0.0, dot(-lightDir, fNormal));

	outputF = (ambient + diffuse) * outputF;

	float shadow_coef = 0.95;
	shadow_coef= shadowCoef();
	outputF = (ambient + (shadow_coef + 0.05) * diffuse) * outputF;

	if (wireframeEnalbed == 1) {
		outputF = edgeIntensity * vec4(0.05, 0.05, 0.05, 1.0) + (1.0 - edgeIntensity) * outputF;
	}
}

