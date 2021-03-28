#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, 
			GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat diffIntensity);
	~Light();

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
					GLfloat diffuseIntensityLocation, GLfloat directionLocation);

private:
	glm::vec3 mColour;
	glm::vec3 mDirection;
	GLfloat mAmbientIntensity, mDiffuseIntensity;
};

