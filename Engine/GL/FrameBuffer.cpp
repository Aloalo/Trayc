#include <Engine/GL/FrameBuffer.h>
#include <Engine/Utils/ErrorCheck.h>
#include <easylogging++.h>

using namespace std;
using namespace glm;

namespace engine
{
    FrameBuffer FrameBuffer::mBackBuffer;
    GLuint FrameBuffer::mBoundFBO = 0;

    static const int maxColorAttachments = 8;
    static const GLenum colorAttachments[maxColorAttachments] =
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

    static inline bool IsColorAttachment(GLenum format) {
        return format - GL_COLOR_ATTACHMENT0 < maxColorAttachments;
    }

    FrameBuffer::FrameBuffer(void)
        : mID(0), mRBID(0), mRBDepth(GL_DEPTH_COMPONENT32), mWidth(0), mHeight(0)
    {
    }

    int FrameBuffer::GetCtColorAttachments() const
    {
        int ct = 0;
        for(const auto &a : mAttachments) {
            if(IsColorAttachment(a.second)) {
                ++ct;
            }
        }
        return ct;
    }

    GLenum FrameBuffer::GetAttachmentType(GLenum format) const
    {
        if(format == GL_DEPTH_COMPONENT) {
            return GL_DEPTH_ATTACHMENT;
        }

        const int colorIdx = GetCtColorAttachments();
        return colorAttachments[colorIdx];
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

    void FrameBuffer::Init()
    {
        if(mID == 0) {
            glGenFramebuffers(1, &mID);
        }
    }

    void FrameBuffer::Destroy()
    {
        for(Attachment &a : mAttachments)
            a.first.Destroy();

        mAttachments.clear();
        glDeleteRenderbuffers(1, &mRBID);
        glDeleteFramebuffers(1, &mID);

        mRBID = mID = 0;
    }

    void FrameBuffer::AddColorAttachment(const Texture2D &tex)
    {
        const GLenum attachment = GetAttachmentType(tex.GetTextureDescription().format);
        mAttachments.push_back(Attachment(tex, attachment));

        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex.Target(), tex.ID(), 0);
        UnBind();
    }

    void FrameBuffer::AddAttachment(GLenum internalFormat, GLenum format, GLenum type)
    {
        const GLenum attachment = GetAttachmentType(format);

        mAttachments.push_back(Attachment(Texture2D(), attachment));
        Texture2D &attachementTex = mAttachments.back().first;
        attachementTex.Init(internalFormat, ivec2(mWidth, mHeight), format, type);

        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, attachementTex.Target(), attachementTex.ID(), 0);
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

    void FrameBuffer::AttachRBO(GLuint RB)
    {
        mRBID = RB;
        Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBID);
        UnBind();
    }

    GLuint FrameBuffer::GetRBOID() const
    {
        return mRBID;
    }

    void FrameBuffer::Clear()
    {
        mAttachments.clear();
    }

    void FrameBuffer::Compile() const
    {
        const int ctColorAttachments = GetCtColorAttachments();

        Bind();
        if(ctColorAttachments > 0) {
            glDrawBuffers(static_cast<GLsizei>(mAttachments.size()), colorAttachments);
        }
        else {
            glDrawBuffer(GL_NONE);
        }
        UnBind();

        // Check completeness
        Check();
    }

    void FrameBuffer::Resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        // Resize color textures
        for(Attachment &a : mAttachments) {
            a.first.Resize(ivec2(mWidth, mHeight));
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

        LOG(ERROR) << "[FrameBuffer::Check] FrameBuffer completeness error: ";
        switch(status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_UNDEFINED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            LOG(ERROR) << "[FrameBuffer::Check] GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;
        default:
            LOG(ERROR) << "[FrameBuffer::Check] Unknown framebuffer error";
            break;
        }
        assert(status == GL_FRAMEBUFFER_COMPLETE);
    }

    void FrameBuffer::Bind() const
    {
        if(mBoundFBO != mID) {
            mBoundFBO = mID;
            glBindFramebuffer(GL_FRAMEBUFFER, mID);
        }
    }

    void FrameBuffer::BindRead() const
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mID);
    }

    void FrameBuffer::BindDraw() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mID);
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
        return mAttachments[idx].first;
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
