/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FRAME_BUFFER_H
#define EN_FRAME_BUFFER_H

#include <GL/glew.h>

namespace engine
{
    class FrameBuffer
    {
    public:
        FrameBuffer(void);
        ~FrameBuffer(void);

        void Init(int width, int height);
        void Resize(int width, int height);
        void Check() const;

        void Bind() const;
        void BindTexture() const;
        static void UnBind();

        int Width() const;
        int Height() const;

    private:
        GLuint mID;
        GLuint mTexID;
        GLuint mRBID;

        int mWidth;
        int mHeight;
    };
}

#endif

