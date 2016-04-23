/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Scene.h>
#include <Engine/Core/Camera.h>

using namespace std;

namespace engine
{
    Scene::Scene(void)
    {
    }

    void Scene::AddObject(const Object3D & obj)
    {
        mObjects3D.push_back(obj);
    }

    int Scene::GetNumObjects() const
    {
        return static_cast<int>(mObjects3D.size());
    }

    const Object3D& Scene::GetObject(int idx) const
    {
        return mObjects3D[idx];
    }

    Object3D& Scene::GetObject(int idx)
    {
        return mObjects3D[idx];
    }

    AABB Scene::GetAABB() const
    {
        AABB ret;
        for(const auto &obj : mObjects3D)
            ret |= obj.GetAABB();
        return ret;
    }

    vector<const Object3D*> Scene::GetObjects(const Camera *camera, bool forForwardPipeline) const
    {
        vector<const Object3D*> ret;
        const Frustum frustum = camera->GetFrustum();

        for(const Object3D &obj : mObjects3D) {
            const Material &mat = mMaterials[obj.GetMaterialIdx()];
            if(mat.mNeedsForwardRender == forForwardPipeline && frustum.Intersect(obj.GetBSphere()) && frustum.Intersect(obj.GetAABB())) {
                ret.push_back(&obj);
            }
        }

        return ret;
    }
}
