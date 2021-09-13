#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "cShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat ambIntensity, GLfloat diffIntensity,
		GLfloat shadowMapWidth, GLfloat shadowMapHeight);
	~Light();

	ShadowMap* GetShadowMap() { return mShadowMap; }

protected:
	glm::vec3 mColour;
	GLfloat mAmbientIntensity, mDiffuseIntensity;

	glm::mat4 mLightProj;

	ShadowMap* mShadowMap;
};

