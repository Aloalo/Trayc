#include <Engine/GL/FrameBuffer.h>
#include <iostream>

using namespace std;

namespace engine
{
    FrameBuffer::FrameBuffer(void) :
        mID(0), mTexID(0), mRBID(0), mWidth(0), mHeight(0)
    {
    }

    FrameBuffer::~FrameBuffer(void)
    {
        glDeleteTextures(1, &mTexID);
        glDeleteRenderbuffers(1, &mRBID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &mID);
    }

    void FrameBuffer::Init(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        // Init texture
        glGenTextures(1, &mTexID);
        glBindTexture(GL_TEXTURE_2D, mTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Init FBO
        glGenFramebuffers(1, &mID);
        glBindFramebuffer(GL_FRAMEBUFFER, mID);

        // Attach texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexID, 0);
        const GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);
        glReadBuffer(GL_NONE);

        // Init RBO
        glGenRenderbuffers(1, &mRBID);
        glBindRenderbuffer(GL_RENDERBUFFER, mRBID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach RBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBID);

        // Check completeness
        Check();
    }

    void FrameBuffer::Resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        // Resize color texture
        glBindTexture(GL_TEXTURE_2D, mTexID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Resize render buffer
        glBindRenderbuffer(GL_RENDERBUFFER, mRBID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void FrameBuffer::Check() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mID);
        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if(status == GL_FRAMEBUFFER_COMPLETE)
            return;

        cerr << "FrameBuffer completeness error: ";
        switch(status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            cerr << "GL_FRAMEBUFFER_UNDEFINED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            cerr << "GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;
        default:
            cerr << "Unknown error";
            break;
        }
        cerr << endl;
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mID);
    }

    void FrameBuffer::BindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D, mTexID);
    }

    void FrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int FrameBuffer::Width() const
    {
        return mWidth;
    }

    int FrameBuffer::Height() const
    {
        return mHeight;
    }
}
