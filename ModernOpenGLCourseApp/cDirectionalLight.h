#pragma once

#include "cLight.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambIntensity, GLfloat diffIntensity,
		GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
		GLuint shadowMapWidth, GLuint shadowMapHeight);
	~DirectionalLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	/// <summary>
	/// 
	/// </summary>
	/// <returns>Returns the mat4 transform of a light.</returns>
	glm::mat4 CalculateLightTransform();


private:
	glm::vec3 mDirection;
};

