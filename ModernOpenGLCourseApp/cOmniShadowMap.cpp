#include "cOmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {}

OmniShadowMap::~OmniShadowMap()
{
}

bool OmniShadowMap::Init(GLuint width, GLuint height)
{
	mShadowWidth = width; mShadowHeight = height;

	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mShadowMap);

	// This is the point in which omnidirectional shadowmaps differ to directional shadowmaps.
	// We can use a cubemap to store the shadowmaps of different perspectives, such as the top face on the cubemap being a shadowmap of above the light source.
	glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);

	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, mShadowWidth, mShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);	// The values from +x to -x, +y to -y, +z to -z, we can cycle through them by starting from +x and incrementing 1 each time.
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	// As we have S and T to represent our X and Y axes, we need R to represent our Z axis.

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mShadowMap, 0);	// Note this time we use glFramebufferTexture, not Texture2D, as we are working with a cubemap.

	// We only work with the depth attachment defined earlier. Setting draw and read to GL_NONE prevents us from working with colours.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer Error: %i\n", status);    // Note: %i refers to an enum
		return false;
	}

	return true;
}

void OmniShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void OmniShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
}
