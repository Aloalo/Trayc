
#ifndef EN_RENDERABLE_OBJECT3D_H
#define EN_RENDERABLE_OBJECT3D_H

#include <Engine/Geometry/Object3D.h>
#include <Engine/GL/VertexArray.h>

namespace engine
{
    class RenderableObject3D
    {
    public:
        RenderableObject3D(void);
        RenderableObject3D(const Object3D *obj);
        ~RenderableObject3D(void);

        bool mIsActive;
        bool mIsMovable;

    private:
        const Object3D *mObj;
        VertexArray mVA;
    };
}

#endif
