/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Geometry/Object3D.h>

namespace engine
{
    Object3D::Object3D(void)
        : mIsActive(true), mIsMovable(false), mDynamicGeometry(false), mMatIdx(-1), mTriMeshIdx(-1), mTransform(1.0f)
    {
    }
}
