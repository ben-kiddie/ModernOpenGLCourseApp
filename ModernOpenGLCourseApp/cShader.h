#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "cDirectionalLight.h"
#include "cPointLight.h"
#include "cSpotLight.h"

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetEyePositionLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();

	void SetDirectionalLight(DirectionalLight* directionalLight);
	/// <summary>
	/// Assign an array of point lights to this shader.
	/// </summary>
	/// <param name="pointLights">A pointer to the array of point lights to be used in this shader.</param>
	/// <param name="lightCount">The number of point lights within the array we point to.</param>
	/// <param name="offset">The offset in which point lights are stored in an array.</param>
	/// <param name="textureUnit">The texture unit to start from when setting point lights.</param>
	void SetPointLights(PointLight* pointLights, unsigned int lightCount, unsigned int offset, unsigned int textureUnit);
	/// <summary>
	/// Assign an array of spot lights to this shader.
	/// </summary>
	/// <param name="spotLights">A pointer to the array of spot lights we are using.</param>
	/// <param name="lightCount">An integer totalling how many spot lights are in the array we point to.</param>
	/// <param name="offset">The offset in which spot lights are stored in an array.</param>
	/// <param name="textureUnit">The texture unit to start from when setting spot lights.</param>
	void SetSpotLights(SpotLight* spotLights, unsigned int lightCount, unsigned int offset, unsigned int textureUnit);
	/// <summary>
	/// Set the active texture unit.
	/// </summary>
	/// <param name="textureUnit">A GLuint indicating the texture unit to activate.</param>
	void SetTexture(GLuint textureUnit);
	/// <summary>
	/// Set the active directional shadow map.
	/// </summary>
	/// <param name="textureUnit">A GLuint indicating the texture unit holding a shadow map to activate.</param>
	void SetDirectionalShadowMap(GLuint textureUnit);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="lightTransform"></param>
	void SetDirectionalLightTransform(glm::mat4* lightTransform);
	/// <summary>
	/// Bind an array of values to the uniformLightMatrices.
	/// </summary>
	/// <param name="lightMatrices">tThe vector array containing values to bind.</param>
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	/// <summary>
	/// 
	/// </summary>
	void UseShader();
	/// <summary>
	/// Clean up the memory on the GPU by deleting shader programs not in use.
	/// </summary>
	void ClearShader();

	/// <summary>
	/// 
	/// </summary>
	void Validate();

private:
	int mPointLightCount;	// Used in looping through how many lights we actually have, as opposed to could have
	GLuint uniformPointLightCount;	// Attach mPointLightCount to this uniform

	int mSpotLightCount;
	GLuint uniformSpotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture, uniformDirectionalShadowMap,
		uniformDirectionalLightTransform,
		uniformOmniLightPosition, uniformFarPlane;

	GLuint uniformLightMatrices[6];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void CompileProgram();
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

