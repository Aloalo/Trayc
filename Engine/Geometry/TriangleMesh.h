/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_TRIANGLEMESH_H
#define EN_TRIANGLEMESH_H

#include <Engine/Geometry/IndexContainer.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
    class TriangleMesh
    {
    public:
        TriangleMesh(void);

        std::vector<glm::vec3> mPositions;
        std::vector<glm::vec3> mNormals;
        std::vector<glm::vec3> mTangents;
        std::vector<glm::vec3> mBiTangents;
        std::vector<glm::vec3> mColors;
        std::vector<glm::vec2> mUVs;
        std::vector<GLuint> mIndices;
    };
}

#endif
