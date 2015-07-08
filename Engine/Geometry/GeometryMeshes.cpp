/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/GeometryMeshes.h>

using namespace std;
using namespace glm;

namespace engine
{
    TriangleMesh GetCubeMesh()
    {
        TriangleMesh ret;

        ret.mPositions = 
        {
            vec3(1.0f, -1.0f, -1.0f),
            vec3(1.0f, -1.0f, 1.0f),
            vec3(-1.0f, -1.0f, 1.0f),
            vec3(-1.0f, -1.0f, -1.0f),
            vec3(1.0f, 1.0f, -1.0f),
            vec3(1.0f, 1.0f, 1.0f),
            vec3(-1.0f, 1.0f, 1.0f),
            vec3(-1.0f, 1.0f, -1.0f)
        };

        return ret;
    }
}
