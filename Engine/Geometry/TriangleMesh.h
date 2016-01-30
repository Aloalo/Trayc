/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_TRIANGLEMESH_H
#define EN_TRIANGLEMESH_H

#include <Engine/Geometry/AABB.h>
#include <Engine/GL/VertexAttributeDef.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
    class TriangleMesh
    {
    public:
        TriangleMesh(void);

        std::vector<glm::vec3> mPositions;
        std::vector<glm::vec2> mUVs;
        std::vector<glm::vec3> mNormals;
        std::vector<glm::vec3> mTangents;
        std::vector<glm::vec3> mBitangents;
        std::vector<unsigned int> mIndices;

        std::vector<VertexAttribDef> GetVertexAttribDefs() const;
        GLenum GetIndexType() const;
        void* GetIndices() const;

        const AABB& GetAABB() const;
        void CalcAABB();

    private:
        friend class Scene;
        AABB mAABB;
    };
}

#endif
