#version 330		
					
struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
};

in vec4 vCol;	
in vec2 vTexCoord;
					
out vec4 colour;	

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
				
void main()			
{					
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	colour = texture(theTexture, vTexCoord) * ambientColour;
}					
