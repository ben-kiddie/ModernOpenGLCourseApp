#include "cDirectionalLight.h"

DirectionalLight::DirectionalLight() 
	: Light()
{
	mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	mLightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat ambIntensity, GLfloat diffIntensity,
	GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
	GLuint shadowMapWidth, GLuint shadowMapHeight)
	: Light(red, green, blue, ambIntensity, diffIntensity, shadowMapWidth, shadowMapHeight)
{
	mDirection = glm::vec3(xDirection, yDirection, zDirection);
	mLightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);	// Directional lights require orthogonal projection, as the rays are not intended to fan out
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, mColour.x, mColour.y, mColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	/*
		The glm::lookAt allows us to visualise where the directional light would be from the world origin. 
		This is because we are reversing the direction of incoming rays of light, effectively tracing back to the origin.
		More info: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	*/
	return mLightProj * glm::lookAt(-mDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	// i.e., projection * view.
}
