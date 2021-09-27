#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "cShader.h"
#include "cMesh.h"

class Skybox
{
public:
	Skybox();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="faceLocations">Vector containing all filepaths of cubemap faces.</param>
	Skybox(std::vector<std::string> faceLocations);
	~Skybox();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="viewMatrix">Location of the camera.</param>
	/// <param name="projectionMatrix">Type of projection used by camera.</param>
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	Mesh* mSkyMesh;
	Shader* mSkyShader;

	GLuint mTextureID;
	GLuint uniformProjection, uniformView;
};

