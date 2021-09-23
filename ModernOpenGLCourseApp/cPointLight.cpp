#include "cPointLight.h"

PointLight::PointLight() 
	: Light()
{
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mConstant = 1.0f;	// At the very least this must be initialised to 1, as the resulting light calculations demand the denominator not be equal to zero
	mLinear = 0.0f;
	mExponent = 0.0f;
}

PointLight::PointLight(	GLfloat red, GLfloat green, GLfloat blue,
						GLfloat ambIntensity, GLfloat diffIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat constant, GLfloat linear, GLfloat exponent,
						GLuint shadowMapWidth, GLuint shadowMapHeight,
						GLfloat nearPlane, GLfloat farPlane)
						: Light(red, green, blue, ambIntensity, diffIntensity, shadowMapWidth, shadowMapHeight)
{
	mPosition = glm::vec3(xPos, yPos, zPos);
	mConstant = constant;
	mLinear = linear;
	mExponent = exponent;
	mFarPlane = farPlane;

	float aspect = (float)shadowMapWidth / (float)shadowMapHeight;
	mLightProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);
	
	mShadowMap = new OmniShadowMap();
	mShadowMap->Init(shadowMapWidth, shadowMapHeight);
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(	GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
							GLuint diffuseIntensityLocation, GLuint positionLocation, 
							GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColourLocation, mColour.x, mColour.y, mColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);

	glUniform3f(positionLocation, mPosition.x, mPosition.y, mPosition.z);
	glUniform1f(constantLocation, mConstant);
	glUniform1f(linearLocation, mLinear);
	glUniform1f(exponentLocation, mExponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;

	// +x, -x
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));	// Note that the direction we look in is reversed.
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	// +y, -y
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));		// Imagine tilting your head upwards - the "up" relative to this new orientation is backwards
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));	// Same applies to tilting downwards. The "up" becomes forward.
	// +z, -z																		  
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


	return lightMatrices;
}
