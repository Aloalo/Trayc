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

        void SetTransform(const glm::mat4 &transform);

        int GetMeshIdx() const;
        int GetMaterialIdx() const;
        AABB GetAABB() const;
        const glm::mat4& GetTransform() const;

        bool mDynamicGeometry;
        bool mShadowCaster;

    private:
        friend class AssetLoader;
        void SetMeshAABB(const AABB *meshAABB);

        int mMeshIdx;
        int mMatIdx;

        const AABB *mMeshAABB;
        glm::mat4 mTransform;
    };
}

#endif
