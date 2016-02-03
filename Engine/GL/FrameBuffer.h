/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FRAME_BUFFER_H
#define EN_FRAME_BUFFER_H

#include <Engine/GL/Texture.h>
#include <GL/glew.h>
#include <vector>

namespace engine
{
    class FrameBuffer
    {
    public:
        FrameBuffer(void);

        void Init(int width, int height);
        void Destroy();

        void AddAttachment(GLenum internalFormat, GLenum format, GLenum type);
        void AttachRBO();
        void Compile() const;
        void Resize(int width, int height);
        void Check() const;

        void BindTexture(int idx) const;
        void Bind() const;
        static void UnBind();

        int Width() const;
        int Height() const;

        const Texture2D& GetAttachment(int idx) const;

    private:
        std::vector<Texture2D> mAttachments;

        GLuint mID;
        GLuint mRBID;

        int mWidth;
        int mHeight;
    };
}

#endif

