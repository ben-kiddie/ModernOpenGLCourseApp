#version 330		

// Constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
				
// Structs
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
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
in vec4 vDirectionalLightSpacePos;

// Output		
out vec4 colour;	

// Uniforms
uniform DirectionalLight directionalLight;
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int spotLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 eyePosition;	// i.e., the camera position for any given first person camera

// Functions
float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = vDirectionalLightSpacePos.xyz / vDirectionalLightSpacePos.w;	// Get normalised device coordinates with range -1 and 1
	projCoords = (projCoords * 0.5f) + 0.5f;	// By halving and adding 0.5f, this gets our values between 0 and 1, which is the same range in which our shadow map depth values operate

	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(light.direction);
	float bias = max(0.05f * (1 - dot(normal, lightDirection)), 0.005f);

	float current = projCoords.z;

	float shadow = 0.0f;

	vec2 texelSize = 1.0f / textureSize(directionalShadowMap, 0);

	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;	// The first and only component available to access is the depth component, which can be stored under .r or .x
			
			shadow += current - bias > pcfDepth ? 1.0f : 0.0f;	// If our current point is greater (i.e., further away) than closest we are in full shadow (1.0f), otherwise not in shadow (0.0f)
		}
	}

	shadow /= 9.0f;
	
	if(projCoords.z > 1.0f)
	{
		shadow = 0.0f;
	}
	
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)	// Directional lights calculate by direction, whereas point lights partly do the same, thus we use this function for both
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	// Remember: a.b = |a| * |b| * cos(theta). Vectors normalised in calculations so the magnitudes become 1, leaving us with a.b = cos(theta).
	float diffuseFactor = max(dot(normalize(vNormal), normalize(direction)), 0.0f);	// Between 0 and 1. Max lets us clamp to an extreme.
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

	vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if(diffuseFactor > 0.0f)	// Considering the fact that any diffuse factor less than 0 means the surface isn't being hit, it stands to reason specular reflections will not occur either
	{
		vec3 vecFragToEye = normalize(eyePosition - vFragPos);	// We just want to know the direction we need to look in for this fragment, so we normalise the result to unit vector (no magnitude involved)
		vec3 vecReflected = normalize(reflect(direction, normalize(vNormal)));
	
		float specularFactor = dot(vecFragToEye, vecReflected);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + (1.0f - shadowFactor) * (diffuseColour + specularColour));	// Shadow factor is calculated as 1.0f being in shadow, however here we need to reverse it - otherwise diffuse and specular will be full power
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight)
{
		vec3 direction = vFragPos - pLight.position;	// Unlike directional light, we don't have a predefined direction, so we must do this dynamicaly per light
		float distance = length(direction);	// Make sure we store the distance before we normalise
		direction = normalize(direction);

		vec4 colour = CalcLightByDirection(pLight.base, direction, 0.0f);
		float attenuation = pLight.exponent * distance * distance +	// ax^2 +
							pLight.linear * distance +				// bx +
							pLight.constant;						// c
		
		return (colour / attenuation);	// Add the current attenuated lighting result to the fragment
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(vFragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);	// Set the spot light factor as the angle between the direction a spot light points and the fragment we are targeting. Used in identifying if a fragment is within our cut-off angle.
	
	// Only calculate the light if it is within our cone of influence
	if(slFactor > sLight.edge)
	{
		return CalcPointLight(sLight.base)							// If we just took the point light calculations as is, we would get hard edges around our spot light.
		* (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));	// The rest of the calculation is used in getting a fade effect around the edge of our spot light.
	}
	else
	{
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);;
	}	
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);	// Holds the sum of attenuated point lights affecting this fragment
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]);	// Add the current attenuated lighting result to the fragment
	}

	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);	// Holds the sum of attenuated point lights affecting this fragment
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i]);	// Add the current attenuated lighting result to the fragment
	}

	return totalColour;
}

void main()			
{					
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();
	colour = texture(theTexture, vTexCoord) * finalColour;
}					
