#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity);
	~Light();

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
};

