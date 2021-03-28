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

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
	glm::vec3 mColour;
	glm::vec3 mDirection;
	GLfloat mAmbientIntensity, mDiffuseIntensity;
};

