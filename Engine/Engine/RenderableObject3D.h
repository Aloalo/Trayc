/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERABLE_OBJECT3D_H
#define EN_RENDERABLE_OBJECT3D_H

#include <Engine/Geometry/Transform.h>
#include <Engine/Geometry/AABB.h>

namespace engine
{
    //Base object for deferred rendering
    class RenderableObject3D
    {
    public:
        RenderableObject3D(void);
        ~RenderableObject3D(void);

        bool mIsActive;

    protected:
        Transform mTransform;
        AABB mBoundingBox;
    };
}

#endif
