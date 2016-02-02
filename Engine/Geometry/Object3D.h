#ifndef EN_OBJECT3D_H
#define EN_OBJECT3D_H

#include <Engine/Geometry/Material.h>
#include <Engine/Geometry/TriangleMesh.h>

namespace engine
{
    //Base object for deferred rendering
    struct Object3D
    {
        Object3D(int meshIdx, int matIdx);

        void SetTransform(const glm::mat4 &transform, const AABB &meshAABB);

        const bool HasDynamicGemoetry() const;
        int GetMeshIdx() const;
        int GetMaterialIdx() const;
        const AABB& GetAABB() const;
        const glm::mat4& GetTransform() const;

    private:
        friend class Scene;
        void CalcAABB(const AABB &meshAABB);

        bool mDynamicGeometry;

        int mMeshIdx;
        int mMatIdx;

        AABB mAABB;
        glm::mat4 mTransform;
    };
}

#endif
