#include "cLight.h"

Light::Light()
{
	mColour = glm::vec3(1.0f, 1.0f, 1.0f);
	mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	mAmbientIntensity = 1.0f;
	mDiffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, 
				GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat diffIntensity)
{
	mColour = glm::vec3(red, green, blue);
	mDirection = glm::vec3(xDirection, yDirection, zDirection);

	mAmbientIntensity = ambIntensity;
	mDiffuseIntensity = diffIntensity;
}

Light::~Light()
{
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, mColour.x, mColour.y, mColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}
