/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MESH_H
#define EN_MESH_H

#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Geometry/Material.h>
#include <vector>

namespace engine
{
    class Scene
    {
    public:
        std::vector<TriangleMesh> meshes;
        std::vector<Material> materials;
    };
}

#endif
