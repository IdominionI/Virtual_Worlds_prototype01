#pragma once

#include "../../Common/pch.h"

/*
                        OpenGL Frame Buffer Class

    This code is an adaptation of the original source code of
    the JGL openGL demo from which the Virtual Worlds application
    used as a basis to evolve from.

    This is code that sets up and initialises and performs the
    various basic OpenGl frame buffer function calls required 
    for OpenGl to function on a basic level.
*/


class openGL_frame_buffer_class{
public:
    openGL_frame_buffer_class() : mFBO{ 0 }, mDepthId{ 0 }{}

    void create_buffers(int32_t width, int32_t height) {
        mWidth = width;
        mHeight = height;

        if (mFBO)
        {
            delete_buffers();
        }

        glGenFramebuffers(1, &mFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
        glCreateTextures(GL_TEXTURE_2D, 1, &mTexId);
        glBindTexture(GL_TEXTURE_2D, mTexId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &mDepthId);
        glBindTexture(GL_TEXTURE_2D, mDepthId);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mWidth, mHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthId, 0);

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(mTexId, buffers);

        unbind();
    }

    void delete_buffers() {
        if (mFBO)
        {
            glDeleteFramebuffers(GL_FRAMEBUFFER, &mFBO);
            glDeleteTextures(1, &mTexId);
            glDeleteTextures(1, &mDepthId);
            mTexId = 0;
            mDepthId = 0;
        }

    }

    void bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
        glViewport(0, 0, mWidth, mHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    uint32_t get_texture() {
        return mTexId;
    }

protected:
    uint32_t mFBO = 0;
    uint32_t mTexId = 0;
    uint32_t mDepthId = 0;
    int32_t  mWidth = 0;
    int32_t  mHeight = 0;
};
