#pragma once

#include "cShadowMap.h"

class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool Init(GLuint width, GLuint height);
	/// <summary>
	/// 
	/// </summary>
	void Write();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="textureUnit"></param>
	void Read(GLenum textureUnit);
};

