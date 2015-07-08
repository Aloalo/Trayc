/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GEOMETRYMESHES_H
#define EN_GEOMETRYMESHES_H

#include <Engine/Geometry/TriangleMesh.h>

namespace engine
{
    // indexed = true, wire = true   -> glDrawElements(GL_LINES, ctIndices, GL_UNSIGNED_BYTE, nullptr)
    // indexed = true, wire = false  -> glDrawElements(GL_TRIANGLES, ctIndices, GL_UNSIGNED_BYTE, nullptr)
    // indexed = false, wire = true  -> glDrawArrays(GL_LINES, 0, ctVertices)
    // indexed = false, wire = false -> glDrawArrays(GL_TRIANGLES, 0, ctVertices)
    TriangleMesh GetCubeMesh(bool indexed, bool wire);
}
#endif
