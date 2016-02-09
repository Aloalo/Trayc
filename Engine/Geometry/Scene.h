/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SCENE_H
#define EN_SCENE_H

#include <Engine/Geometry/Object3D.h>
#include <Engine/Geometry/Material.h>
#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Engine/Light.h>

namespace engine
{
    class Scene
    {
    public:
        Scene(void);

        AABB GetAABB() const;

        std::vector<Light> mLights;
        std::vector<Object3D> mObjects3D;
        std::vector<TriangleMesh> mTriMeshes;
        std::vector<Material> mMaterials;
    };
}

#endif
