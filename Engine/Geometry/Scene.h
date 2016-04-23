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
    class Camera;

    class Scene
    {
    public:
        Scene(void);

        void AddObject(const Object3D &obj);

        int GetNumObjects() const;
        const Object3D& GetObject(int idx) const;
        Object3D& GetObject(int idx);
        AABB GetAABB() const;
        std::vector<const Object3D*> GetObjects(const Camera *camera, bool forForwardPipeline) const;

        std::vector<Object3D> mObjects3D;
        std::vector<const Light*> mLights;
        std::vector<TriangleMesh> mTriMeshes;
        std::vector<Material> mMaterials;
    };
}

#endif
