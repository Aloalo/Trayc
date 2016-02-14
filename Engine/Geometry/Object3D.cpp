/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Geometry/Object3D.h>

using namespace glm;

namespace engine
{
    Object3D::Object3D(int meshIdx, int matIdx)
        : mDynamicGeometry(false), mShadowCaster(true), mMeshIdx(meshIdx), mMatIdx(matIdx), mMeshAABB(nullptr), mTransform(1.0f)
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

    const mat4& Object3D::GetTransform() const
    {
        return mTransform;
    }

    void Object3D::SetMeshAABB(const AABB *meshAABB)
    {
        mMeshAABB = meshAABB;
    }
}
