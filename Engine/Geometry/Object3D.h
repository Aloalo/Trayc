#ifndef EN_OBJECT3D_H
#define EN_OBJECT3D_H

#include <Engine/Geometry/AABB.h>

namespace engine
{
    //Base object for deferred rendering
    struct Object3D
    {
        Object3D(void);

        bool mIsActive;
        bool mIsMovable;
        bool mDynamicGeometry;

        int mMatIdx;
        int mTriMeshIdx;

        glm::mat4 mTransform;
    };
}

#endif
