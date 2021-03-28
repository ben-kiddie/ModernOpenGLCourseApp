#version 330		
				
// Structs
struct DirectionalLight
{
	vec3 colour;
	vec3 direction;
	float ambientIntensity;
	float diffuseIntensity;
};

// Input
in vec4 vCol;	
in vec2 vTexCoord;
in vec3 vNormal;

// Output		
out vec4 colour;	

// Uniforms
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

// Functions				
void main()			
{					
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// Remember: a.b = |a| * |b| * cos(theta). Vectors normalised in calculations so the magnitudes become 1, leaving us with a.b = cos(theta).
	float diffuseFactor = max(dot(normalize(vNormal), normalize(directionalLight.direction)), 0.0f);	// Between 0 and 1. Max lets us clamp to an extreme.

	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	colour = texture(theTexture, vTexCoord) * ambientColour + diffuseColour;
}					
