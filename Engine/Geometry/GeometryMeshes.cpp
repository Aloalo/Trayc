/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/GeometryMeshes.h>

using namespace std;
using namespace glm;

namespace engine
{
    template<typename T, int sz>
    int size(T(&)[sz])
    {
        return sz;
    }

    static const vec3 positionsCube[] =
    {
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f, -1.0f, 1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(-1.0f, -1.0f, -1.0f),
    };

    static const GLubyte indicesCubeWire[] =
    {
        0, 1, 0, 2, 0, 4,
        3, 1, 3, 2, 3, 7,
        5, 1, 5, 7, 5, 4,
        6, 2, 6, 7, 6, 4
    };

    static const GLubyte indicesCubeSolid[] =
    {
        0, 3, 1, 0, 2, 3,
        0, 5, 4, 0, 1, 5,
        0, 4, 6, 0, 6, 2,
        7, 1, 3, 7, 5, 1,
        7, 3, 2, 7, 2, 6,
        7, 4, 5, 7, 6, 4
    };

    TriangleMesh GetCubeMesh(bool indexed, bool wire) //TODO: normals, UVs
    {
        const GLubyte *indices = (wire ? indicesCubeWire : indicesCubeSolid);
        const int ctIndices = (wire ? size(indicesCubeWire) : size(indicesCubeSolid));
        const int ctVertices = size(positionsCube);

        TriangleMesh ret;
        if(indexed)
        {
            ret.mIndices.insert(ret.mIndices.begin(), indices, indices + ctIndices);
            ret.mPositions.insert(ret.mPositions.begin(), positionsCube, positionsCube + ctVertices);
        }
        else
        {
            for(int i = 0; i < ctIndices; ++i)
                ret.mPositions.push_back(positionsCube[indices[i]]);
        }

        return ret;
    }

}
