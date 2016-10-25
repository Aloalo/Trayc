/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SCENE_H
#define EN_SCENE_H

#include <Engine/Geometry/Object3D.h>
#include <Engine/Geometry/Material.h>
#include <Engine/Geometry/TriangleMesh.h>
#include <Engine/Engine/Light.h>
#include <Engine/Core/Camera.h>

namespace engine
{
    class Camera;

    class Scene
    {
    public:
        Scene(void);

        void Scale(float scale);

        void AddObject(const Object3D &obj);

        int GetNumObjects() const;
        const Object3D& GetObject(int idx) const;
        Object3D& GetObject(int idx);
        AABB GetAABB() const;

        std::vector<const Object3D*> GetShadowCasters() const;

        template<bool forForwardPipeline>
        std::vector<const Object3D*> GetObjects(const Frustum &frustum) const
        {
            std::vector<const engine::Object3D*> ret;
            ret.reserve(mObjects3D.size());

            for(const Object3D &obj : mObjects3D) {
                if(obj.mVisible) {
                    const Material &mat = mMaterials[obj.GetMaterialIdx()];
                    if(mat.mNeedsForwardRender == forForwardPipeline && frustum.Intersect(obj.GetBSphere()) && frustum.Intersect(obj.GetAABB())) {
                        ret.push_back(&obj);
                    }
                }
            }

            return ret;
        }

        std::vector<Object3D> mObjects3D;
        std::vector<const Light*> mLights;
        std::vector<TriangleMesh> mTriMeshes;
        std::vector<Material> mMaterials;
    };
}

#endif
