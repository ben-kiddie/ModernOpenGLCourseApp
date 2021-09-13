#pragma once

#include <stdio.h>	// Output errors with our FBO
#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	/// <summary>
	/// Initialise a texture for our shadow map.
	/// </summary>
	/// <param name="width">Width of the shadow map texture.</param>
	/// <param name="height">Height of the shadow map texture.</param>
	/// <returns></returns>
	virtual bool Init(GLuint width, GLuint height);
	/// <summary>
	/// Bind the framebuffer object used to write to a shadow map texture.
	/// </summary>
	virtual void Write();
	/// <summary>
	/// Read the shadow map texture as a point of reference when rendering on screen.
	/// </summary>
	/// <param name="textureUnit">The unit which we want a shadow map to be stored at, so that texture sampler uniform variables can access this.</param>
	virtual void Read(GLenum textureUnit);

	GLuint GetShadowWidth() { return mShadowWidth; }
	GLuint GetShadowHeight() { return mShadowHeight; }

protected:
	GLuint mFBO,	
		mShadowMap, 
		mShadowWidth, mShadowHeight;
};

