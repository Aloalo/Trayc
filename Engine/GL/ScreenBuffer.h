/*
* Copyright (c) 2014 Jure Ratkovic
*/


#ifndef EN_SCREEN_BUFFER_H
#define EN_SCREEN_BUFFER_H

#include <Engine/GL/FrameBuffer.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>

namespace engine
{
    class ScreenBuffer
    {
    public:
        ScreenBuffer(void);
        ~ScreenBuffer(void);

        void Init(int width, int height);
        void Resize(int width, int height);

        void SetAsRenderTarget() const;

        void Render(int width, int height) const;

    private:
        FrameBuffer mFBO;
        Program mProgram;
        VertexArray mVAO;
    };
}

#endif
