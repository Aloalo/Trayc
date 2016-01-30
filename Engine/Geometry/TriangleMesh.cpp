#include <Engine/Geometry/TriangleMesh.h>

using namespace glm;
using namespace std;

namespace engine
{
    TriangleMesh::TriangleMesh(void)
    {
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
    static inline void* CastIndices(const vector<unsigned int> mIndices)
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
