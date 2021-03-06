
#ifndef EN_FRAME_BUFFER_H
#define EN_FRAME_BUFFER_H

#include <Engine/GL/Texture2D.h>
#include <GL/glew.h>
#include <vector>

namespace engine
{
    class FrameBuffer
    {
        using Attachment = std::pair<Texture2D, GLenum>;
    public:
        FrameBuffer(void);

        // Use this if Framebuffer is static
        void Init(int width, int height);
        // Use this if Framebuffer is dynamic (changes textures, doesn't have a size)
        void Init();
        void Destroy();

        void AddColorAttachment(const Texture2D &tex);
        void AddAttachment(GLenum internalFormat, GLenum format, GLenum type);
        // Creates and attaches a renderbuffer
        void AttachRBO();
        // Attaches a renderbuffer
        void AttachRBO(GLuint RB);
        GLuint GetRBOID() const;

        // Clears attachments without deleting the textures
        void Clear();

        // drawBuffers = are the attached textures color targets
        void Compile() const;
        void Resize(int width, int height);
        void Check() const;

        void Bind() const;
        void BindRead() const;
        void BindDraw() const;
        static void UnBind();

        int Width() const;
        int Height() const;

        const Texture2D& GetAttachment(int idx) const;

        bool Exists() const;

        static const FrameBuffer& BackBuffer();

    private:
        int GetCtColorAttachments() const;
        GLenum GetAttachmentType(GLenum format) const;

        std::vector<Attachment> mAttachments;

        GLuint mID;
        GLuint mRBID;
        GLenum mRBDepth;

        int mWidth;
        int mHeight;

        static FrameBuffer mBackBuffer;
        static GLuint mBoundFBO;
    };
}

#endif

