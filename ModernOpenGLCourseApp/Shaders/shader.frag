#version 330		
				
// Structs
struct DirectionalLight
{
	vec3 colour;
	vec3 direction;
	float ambientIntensity;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

// Input
in vec4 vCol;	
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

// Output		
out vec4 colour;	

// Uniforms
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;	// i.e., the camera position for any given first person camera

// Functions				
void main()			
{					
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// Remember: a.b = |a| * |b| * cos(theta). Vectors normalised in calculations so the magnitudes become 1, leaving us with a.b = cos(theta).
	float diffuseFactor = max(dot(normalize(vNormal), normalize(directionalLight.direction)), 0.0f);	// Between 0 and 1. Max lets us clamp to an extreme.
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if(diffuseFactor > 0)	// Considering the fact that any diffuse factor less than 0 means the surface isn't being hit, it stands to reason specular reflections will not occur either
	{
		vec3 vecFragToEye = normalize(eyePosition - vFragPos);	// We just want to know the direction we need to look in for this fragment, so we normalise the result to unit vector (no magnitude involved)
		vec3 vecReflected = normalize(reflect(directionalLight.direction, normalize(vNormal)));
	
		float specularFactor = dot(vecFragToEye, vecReflected);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	colour = texture(theTexture, vTexCoord) * (ambientColour + diffuseColour + specularColour);
}					
