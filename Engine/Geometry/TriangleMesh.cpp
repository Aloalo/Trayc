#include <Engine/Geometry/TriangleMesh.h>

using namespace glm;

namespace engine
{
    TriangleMesh::TriangleMesh(void)
    {
    }

    const AABB & TriangleMesh::GetAABB() const
    {
        return mAABB;
    }

    void TriangleMesh::CalcAABB()
    {
        AABB newAABB;
        for(const vec3 &p : mPositions)
            newAABB |= p;
        mAABB = newAABB;
    }
}
