/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Geometry/Object3D.h>

using namespace glm;

namespace engine
{
    Object3D::Object3D(int meshIdx, int matIdx)
        : mDynamicGeometry(false), mMeshIdx(meshIdx), mMatIdx(matIdx), mTransform(1.0f)
    {
    }

    void Object3D::SetTransform(const mat4 &transform, const AABB &meshAABB)
    {
        mTransform = transform;
        CalcAABB(meshAABB);
    }

    const bool Object3D::HasDynamicGemoetry() const
    {
        return mDynamicGeometry;
    }

    int Object3D::GetMeshIdx() const
    {
        return mMeshIdx;
    }

    int Object3D::GetMaterialIdx() const
    {
        return mMatIdx;
    }

    const AABB& Object3D::GetAABB() const
    {
        return mAABB;
    }

    const mat4& Object3D::GetTransform() const
    {
        return mTransform;
    }

    void Object3D::CalcAABB(const AABB &meshAABB)
    {
        auto vertices = meshAABB.Vertices();

        const int ctVertices = vertices.size();
        for(int i = 0; i < ctVertices; ++i)
            vertices[i] = vec3(mTransform * vec4(vertices[i], 1.0f));

        mAABB = AABB();
        mAABB |= vertices;
    }
}
