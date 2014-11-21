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

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::vector<glm::vec2> uvs;
        std::vector<glm::ivec3> indices;

        //index to the scene's material array
        int materialIndex;
        GLenum orientation;

        //glm::mat4 transform;
        //bool isStatic;
    };
}

#endif
