/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GEOMETRYMESHES_H
#define EN_GEOMETRYMESHES_H

#include <Engine/Geometry/TriangleMesh.h>

namespace engine
{
    // All returned meshes are centered around (0, 0, 0) and have AABB size of (2, 2)
    TriangleMesh GetCubeMeshSolid(bool indexed);
    TriangleMesh GetCubeMeshWire(bool indexed);
    // quality in [0, 3]
    TriangleMesh GetSphereMeshSolid(bool indexed, int quality);

}
#endif
