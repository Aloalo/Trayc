/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FRAME_BUFFER_H
#define EN_FRAME_BUFFER_H

#include <GL/glew.h>
#include <vector>

namespace engine
{
    class FrameBuffer
    {
    public:
        struct FBAttachment
        {
            friend class FrameBuffer;

            FBAttachment(GLenum format, GLenum internalFormat, GLenum type, float scale);

            GLenum mFormat;
            GLenum mInternalFormat;
            GLenum mType;
            float mScale; // Scale to the buffers width/height

        private:
            GLuint mID;
        };


        FrameBuffer(int width, int height);
        ~FrameBuffer(void);

        void AddAttachment(const FBAttachment &fba);
        void AttachRBO();
        void Compile() const;
        void Resize(int width, int height);
        void Check() const;

        void BindTexture(int idx) const;
        void Bind() const;
        static void UnBind();

        int Width() const;
        int Height() const;

    private:
        std::vector<FBAttachment> mAttachments;

        GLuint mID;
        GLuint mRBID;

        int mWidth;
        int mHeight;
    };
}

#endif

