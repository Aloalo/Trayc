#include <Engine/Geometry/TriangleMesh.h>
#include <glm/gtx/norm.hpp>

using namespace glm;
using namespace std;

namespace engine
{
    TriangleMesh::TriangleMesh(unsigned int drawMode)
        : mDrawMode(drawMode)
    {
    }

    void TriangleMesh::Clear()
    {
        mPositions.clear();
        mUVs.clear();
        mNormals.clear();
        mTangents.clear();
        mBitangents.clear();
        mIndices.clear();
        mAABB = AABB();
    }

    void TriangleMesh::FlipNormals()
    {
        for(vec3 &n : mNormals)
            n = -n;
    }

    vector<VertexAttribDef> TriangleMesh::GetVertexAttribDefs() const
    {
        vector<VertexAttribDef> ret;

        if(mPositions.size() > 0)
        {
            ret.push_back(VertexAttribDef(0, 3, GL_FLOAT, GL_FALSE));

            if(mUVs.size() > 0) {
                ret.push_back(VertexAttribDef(1, 2, GL_FLOAT, GL_FALSE));
            }

            if(mNormals.size() > 0) {
                ret.push_back(VertexAttribDef(2, 3, GL_FLOAT, GL_FALSE));
            }

            if(mTangents.size() > 0) {
                ret.push_back(VertexAttribDef(3, 3, GL_FLOAT, GL_FALSE));
            }

            if(mBitangents.size() > 0) {
                ret.push_back(VertexAttribDef(4, 3, GL_FLOAT, GL_FALSE));
            }
        }

        return ret;
    }

    GLenum TriangleMesh::GetIndexType() const
    {
        const int ctPositions = mPositions.size();
        if(ctPositions < 256)
            return GL_UNSIGNED_BYTE;
        else if(ctPositions < 65536)
            return GL_UNSIGNED_SHORT;

        return GL_UNSIGNED_INT;
    }

    template<typename T>
    static inline void* CastIndices(const vector<unsigned int> &mIndices)
    {
        const int ctIndices = mIndices.size();

        T *indices = new T[ctIndices];

        for(int i = 0; i < ctIndices; ++i)
            indices[i] = static_cast<T>(mIndices[i]);

        return static_cast<void*>(indices);
    }

    void* TriangleMesh::GetIndices() const
    {
        if(mIndices.size() > 0)
        {
            const int ctPositions = mPositions.size();

            if(ctPositions < 256)
                return CastIndices<unsigned char>(mIndices);
            else if(ctPositions < 65536)
                return CastIndices<unsigned short>(mIndices);
            else
                return CastIndices<unsigned int>(mIndices);
        }
        return nullptr;
    }

    vector<float> TriangleMesh::GetVertexArray() const
    {
        const int ctVertices = mPositions.size();
        vector<float> ret;
        ret.reserve(ctVertices * GetVertexSize());

        const bool hasUV = mUVs.size() > 0;
        const bool hasNormals = mNormals.size() > 0;
        const bool hasTangents = mTangents.size() > 0;
        const bool hasBitangents = mBitangents.size() > 0;

        for(int i = 0; i < ctVertices; ++i)
        {
            ret.push_back(mPositions[i].x);
            ret.push_back(mPositions[i].y);
            ret.push_back(mPositions[i].z);

            if(hasUV) {
                ret.push_back(mUVs[i].x);
                ret.push_back(mUVs[i].y);
            }

            if(hasNormals) {
                ret.push_back(mNormals[i].x);
                ret.push_back(mNormals[i].y);
                ret.push_back(mNormals[i].z);
            }

            if(hasTangents) {
                ret.push_back(mTangents[i].x);
                ret.push_back(mTangents[i].y);
                ret.push_back(mTangents[i].z);
            }

            if(hasBitangents) {
                ret.push_back(mBitangents[i].x);
                ret.push_back(mBitangents[i].y);
                ret.push_back(mBitangents[i].z);
            }
        }

        return ret;
    }

    int TriangleMesh::GetVertexSize() const
    {
        int ret = 0;

        if(mPositions.size() > 0)
        {
            ret += 3;

            if(mUVs.size() > 0) {
                ret += 2;
            }

            if(mNormals.size() > 0) {
                ret += 3;
            }

            if(mTangents.size() > 0) {
                ret += 3;
            }

            if(mBitangents.size() > 0) {
                ret += 3;
            }
        }

        return ret;
    }

    int TriangleMesh::GetVertexSizeInBytes() const
    {
        return GetVertexSize() * sizeof(float);
    }

    const AABB& TriangleMesh::GetAABB() const
    {
        return mAABB;
    }

    const Sphere& TriangleMesh::GetBSphere() const
    {
        return mBSphere;
    }

    void TriangleMesh::CalcBoundingVolumes()
    {
        AABB newAABB;
        for(const vec3 &p : mPositions)
            newAABB |= p;
        mAABB = newAABB;

        const int ctPos = mPositions.size();
        vec3 a, b;
        float maxDistSq = -1.0f;
        for(int i = 0; i < ctPos; ++i) {
            const vec3 p = mPositions[i];
            for(int j = i + 1; j < ctPos; ++j) {
                const float distSq = distance2(p, mPositions[j]);
                if(distSq > maxDistSq) {
                    a = p;
                    b = mPositions[j];
                    maxDistSq = distSq;
                }
            }
        }

        mBSphere.mCenter = (a + b) * 0.5f;
        mBSphere.mRadius = sqrtf(maxDistSq) * 0.5f;
    }

    unsigned int TriangleMesh::GetDrawMode() const
    {
        return mDrawMode;
    }

    void TriangleMesh::SetBoundingVolumes(const AABB & box, const Sphere & bSphere)
    {
        mAABB = box;
        mBSphere = bSphere;
    }
}
