#ifndef EN_OBJECT3D_H
#define EN_OBJECT3D_H

#include <Engine/Geometry/Material.h>
#include <Engine/Geometry/TriangleMesh.h>

namespace engine
{
    //Base object for deferred rendering
    struct Object3D
    {
        Object3D(TriangleMesh &triMesh, Material &material);

        void SetTransform(const glm::mat4 &transform);

        const bool HasDynamicGemoetry() const;
        const TriangleMesh& GetMesh() const;
        const Material& GetMaterial() const;
        const AABB& GetAABB() const;
        const glm::mat4& GetTransform() const;

    private:
        friend class Scene;
        void CalcAABB();

        bool mDynamicGeometry;

        TriangleMesh &mTriMesh;
        Material &mMaterial;

        AABB mAABB;
        glm::mat4 mTransform;
    };
}

#endif
