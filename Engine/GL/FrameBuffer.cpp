#include <Engine/GL/FrameBuffer.h>
#include <easylogging++.h>

using namespace std;
using namespace glm;

namespace engine
{
    FrameBuffer FrameBuffer::mBackBuffer;
    GLuint FrameBuffer::mBoundFBO = 0;

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
        : mID(0), mRBID(0), mRBDepth(GL_DEPTH_COMPONENT32), mWidth(0), mHeight(0)
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
            fba.Destroy();

        glDeleteRenderbuffers(1, &mRBID);
        glDeleteFramebuffers(1, &mID);
    }

    void FrameBuffer::AddAttachment(GLenum internalFormat, GLenum format, GLenum type)
    {
        mAttachments.push_back(Texture2D());
        Texture2D &attachement = mAttachments.back();
        attachement.Init(internalFormat, ivec2(mWidth, mHeight), format, type);

        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[mAttachments.size() - 1], GL_TEXTURE_2D, attachement.ID(), 0);
        UnBind();
    }

    void FrameBuffer::AttachRBO()
    {
        if(mRBID == 0)
        {
            // Init RBO
            glGenRenderbuffers(1, &mRBID);
            glBindRenderbuffer(GL_RENDERBUFFER, mRBID);
            glRenderbufferStorage(GL_RENDERBUFFER, mRBDepth, mWidth, mHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // Attach RBO
            Bind();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBID);
            UnBind();
        }
    }

    void FrameBuffer::Compile() const
    {
        Bind();
        glDrawBuffers(mAttachments.size(), colorAttachments);
        UnBind();

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
            glRenderbufferStorage(GL_RENDERBUFFER, mRBDepth, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }

    void FrameBuffer::Check() const
    {
        Bind();
        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        UnBind();
        if(status == GL_FRAMEBUFFER_COMPLETE)
            return;

        LOG(ERROR) << "FrameBuffer completeness error: ";
        switch(status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            LOG(ERROR) << "GL_FRAMEBUFFER_UNDEFINED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOG(ERROR) << "GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            LOG(ERROR) << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;
        default:
            LOG(ERROR) << "Unknown framebuffer error";
            break;
        }
    }

    void FrameBuffer::BindTexture(int idx) const
    {
        glBindTexture(GL_TEXTURE_2D, mAttachments[idx].ID());
    }

    void FrameBuffer::Bind() const
    {
        if(mBoundFBO != mID) {
            mBoundFBO = mID;
            glBindFramebuffer(GL_FRAMEBUFFER, mID);
        }
    }

    void FrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        mBoundFBO = 0;
    }

    int FrameBuffer::Width() const
    {
        return mWidth;
    }

    int FrameBuffer::Height() const
    {
        return mHeight;
    }

    const Texture2D& FrameBuffer::GetAttachment(int idx) const
    {
        return mAttachments[idx];
    }

    bool FrameBuffer::Exists() const
    {
        return mID != 0;
    }

    const FrameBuffer& FrameBuffer::BackBuffer()
    {
        return mBackBuffer;
    }
}
