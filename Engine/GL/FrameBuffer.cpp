#include <Engine/GL/FrameBuffer.h>
#include <iostream>

using namespace std;

namespace engine
{
    FrameBuffer::FBAttachment::FBAttachment(GLenum format, GLenum internalFormat, GLenum type, float scale)
        :mFormat(format), mInternalFormat(internalFormat), mType(type), mScale(scale), mID(0)
    {
    }

    static const GLenum colorAttachments[8] = 
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
        GL_COLOR_ATTACHMENT4,
        GL_COLOR_ATTACHMENT5,
        GL_COLOR_ATTACHMENT6,
        GL_COLOR_ATTACHMENT7
    };

    FrameBuffer::FrameBuffer(int width, int height) :
        mID(0), mRBID(0), mWidth(width), mHeight(height)
    {
        glGenFramebuffers(1, &mID);
    }

    FrameBuffer::~FrameBuffer(void)
    {
        for(FBAttachment &fba : mAttachments)
            glDeleteTextures(1, &fba.mID);

        glDeleteRenderbuffers(1, &mRBID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &mID);
    }

    void FrameBuffer::AddAttachment(const FBAttachment &fba)
    {
        mAttachments.push_back(FBAttachment(fba));
        FBAttachment &mFBA = mAttachments.back();

        glGenTextures(1, &mFBA.mID);
        glBindTexture(GL_TEXTURE_2D, mFBA.mID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        const int width = int(float(mWidth) * mFBA.mScale);
        const int height = int(float(mHeight) * mFBA.mScale);
        glTexImage2D(GL_TEXTURE_2D, 0, mFBA.mFormat, width, height, 0, mFBA.mInternalFormat, mFBA.mType, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, mID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[mAttachments.size() - 1], GL_TEXTURE_2D, mFBA.mID, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::AttachRBO()
    {
        // Init RBO
        glGenRenderbuffers(1, &mRBID);
        glBindRenderbuffer(GL_RENDERBUFFER, mRBID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mWidth, mHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach RBO
        glBindFramebuffer(GL_FRAMEBUFFER, mID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Compile() const
    {
        glDrawBuffers(mAttachments.size(), colorAttachments);
        glReadBuffer(GL_NONE);

        // Check completeness
        Check();
    }

    void FrameBuffer::Resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        // Resize color textures
        for(const FBAttachment &fba : mAttachments)
        {
            const int width = int(float(mWidth) * fba.mScale);
            const int height = int(float(mHeight) * fba.mScale);
            glBindTexture(GL_TEXTURE_2D, fba.mID);
            glTexImage2D(GL_TEXTURE_2D, 0, fba.mFormat, width, height, 0, fba.mInternalFormat, fba.mType, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if(mRBID != 0)
        {
            // Resize render buffer
            glBindRenderbuffer(GL_RENDERBUFFER, mRBID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
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

    void FrameBuffer::BindTexture(int idx) const
    {
        glBindTexture(GL_TEXTURE_2D, mAttachments[idx].mID);
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mID);
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
