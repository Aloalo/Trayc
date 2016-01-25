/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/RenderableObject3D.h>

namespace engine
{
    RenderableObject3D::RenderableObject3D(void)
        : mIsActive(true), mIsMovable(false), mDynamicGeometry(false)
    {
    }

    RenderableObject3D::~RenderableObject3D(void)
    {
    }
}
