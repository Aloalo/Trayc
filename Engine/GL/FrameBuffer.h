/*
* Copyright (c) 2014 Jure Ratkovic
*/

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

        void Init(int width, int height);
        void Destroy();

        void AddAttachment(GLenum internalFormat, GLenum format, GLenum type);
        // Creates and attaches a renderbuffer
        void AttachRBO();
        // Attaches a renderbuffer
        void AttachRBO(GLuint RB);
        GLuint GetRBOID() const;
        // drawBuffers = are the attached textures color targets
        void Compile() const;
        void Resize(int width, int height);
        void Check() const;

        void BindTexture(int idx) const;
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

