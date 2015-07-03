/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderable.h>

namespace engine
{
    Renderable::Renderable(void)
        : mIsActive(true), mVAO(0)
    {
        glGenVertexArrays(1, &mVAO);
    }

    Renderable::~Renderable(void)
    {
        glDeleteVertexArrays(1, &mVAO);
    }

}
