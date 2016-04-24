#ifndef EN_OBJECT3D_H
#define EN_OBJECT3D_H

#include <Engine/Geometry/Material.h>
#include <Engine/Geometry/TriangleMesh.h>

namespace engine
{
    //Base object for deferred rendering
    //Constructed from AssetLoader
    struct Object3D
    {
        void SetTransform(const glm::mat4 &transform);

        int GetMeshIdx() const;
        int GetMaterialIdx() const;
        AABB GetAABB() const;
        Sphere GetBSphere() const;
        const glm::mat4& GetTransform() const;

        bool mDynamicGeometry;
        bool mShadowCaster;

    private:
        friend class AssetLoader;

        Object3D(int meshIdx, int matIdx);
        void SetBoundingVolumes(const AABB *meshAABB, const Sphere *meshBSphere);

        int mMeshIdx;
        int mMatIdx;

        const AABB *mMeshAABB;
        const Sphere *mMeshBSphere;
        glm::mat4 mTransform;
    };
}

#endif
