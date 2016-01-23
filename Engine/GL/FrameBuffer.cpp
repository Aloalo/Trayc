#include <Engine/GL/FrameBuffer.h>
#include <iostream>

using namespace std;
using namespace glm;

namespace engine
{
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

    FrameBuffer::FrameBuffer(void)
        : mID(0), mRBID(0), mWidth(0), mHeight(0)
    {
    }

    void FrameBuffer::Init(int width, int height)
    {
        if(mID == 0)
        {
            mWidth = width;
            mHeight = height;
            glGenFramebuffers(1, &mID);
        }
    }

    void FrameBuffer::Destroy()
    {
        for(Texture2D &fba : mAttachments)
            fba.Delete();

        glDeleteRenderbuffers(1, &mRBID);
        glDeleteFramebuffers(1, &mID);
    }

    void FrameBuffer::AddAttachment(GLenum internalFormat, GLenum format, GLenum type)
    {
        mAttachments.push_back(Texture2D());
        Texture2D &attachement = mAttachments.back();
        attachement.Init(internalFormat, ivec2(mWidth, mHeight), format, type);

        glBindFramebuffer(GL_FRAMEBUFFER, mID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[mAttachments.size() - 1], GL_TEXTURE_2D, attachement.ID(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::AttachRBO()
    {
        if(mRBID == 0)
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
    }

    void FrameBuffer::Compile() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mID);
        glDrawBuffers(mAttachments.size(), colorAttachments);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Check completeness
        Check();
    }

    void FrameBuffer::Resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        // Resize color textures
        for(Texture2D &fba : mAttachments) {
            fba.Resize(ivec2(mWidth, mHeight));
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
        glBindTexture(GL_TEXTURE_2D, mAttachments[idx].ID());
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
