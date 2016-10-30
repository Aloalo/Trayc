
#include <Engine/Geometry/Object3D.h>

using namespace glm;

namespace engine
{
    Object3D::Object3D(int meshIdx, int matIdx)
        : mDynamicGeometry(false), mShadowCaster(true), mVisible(true), mMeshIdx(meshIdx),
        mMatIdx(matIdx), mMeshAABB(nullptr), mMeshBSphere(nullptr), mTransform(1.0f)
    {
    }

    void Object3D::SetTransform(const mat4 &transform)
    {
        mTransform = transform;
    }

    int Object3D::GetMeshIdx() const
    {
        return mMeshIdx;
    }

    int Object3D::GetMaterialIdx() const
    {
        return mMatIdx;
    }

    AABB Object3D::GetAABB() const
    {
        auto vertices = mMeshAABB->Vertices();

        const int ctVertices = vertices.size();
        for(int i = 0; i < ctVertices; ++i)
            vertices[i] = vec3(mTransform * vec4(vertices[i], 1.0f));

        AABB mRetAABB;
        mRetAABB |= vertices;

        return mRetAABB;
    }

    Sphere Object3D::GetBSphere() const
    {
        return Sphere(vec3(mTransform * vec4(mMeshBSphere->mCenter, 1.0f)), mMeshBSphere->mRadius);
    }

    const mat4& Object3D::GetTransform() const
    {
        return mTransform;
    }

    void Object3D::SetBoundingVolumes(const AABB *meshAABB, const Sphere *meshBSphere)
    {
        mMeshAABB = meshAABB;
        mMeshBSphere = meshBSphere;
    }
}
