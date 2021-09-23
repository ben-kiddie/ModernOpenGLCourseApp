#include "cShadowMap.h"

ShadowMap::ShadowMap()
{
    mFBO = 0;
    mShadowMap = 0;
    mShadowWidth = 0.0f; mShadowHeight = 0.0f;
}

ShadowMap::~ShadowMap()
{
    if (mFBO) {
        glDeleteFramebuffers(1, &mFBO);
    }

    if (mShadowMap) {
        glDeleteTextures(1, &mShadowMap);
    }
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
    mShadowWidth = width, mShadowHeight = height;

    glGenFramebuffers(1, &mFBO);    // Create an FBO to draw to and save as texture
    
    glGenTextures(1, &mShadowMap);  // Create a texture to hold the contents saved to our custom FBO
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowWidth, mShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);  // Setup one massive empty texture consisting of depth component coordinates, which will be bound to our FBO and updated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       // Use GL_LINEAR for more blobby shadows, GL_NEAREST for pixelated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);    // Note: GL_FRAMEBUFFER can be replaced with GL_READ_FRAMEBUFFER or GL_WRITE_FRAMEBUFFER depending on what you want to do.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);   // Attach our texture to the custom FBO so that our shadow map is updated. Note: GL_DEPTH_ATTACHMENT specifies we are writing our depth map to the texture.

    // We just want the depth data, no need to read or write from the colour buffer.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer Error: %i\n", status);    // Note: %i refers to an enum
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void ShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);    // Write to our custom offscreen buffer
}

void ShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
