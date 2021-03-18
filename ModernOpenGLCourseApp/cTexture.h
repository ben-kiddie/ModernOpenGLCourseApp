#pragma once

#include <GL/glew.h>
#include <stb/stb_image.h>

// https://www.textures.com/ - texture repository

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint mTextureID;
	int mWidth, mHeight, mBitDepth;
	const char* mFileLocation;
};

