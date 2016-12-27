
#include <Engine/Geometry/Scene.h>
#include <Engine/Core/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace engine
{
    Scene::Scene(void)
    {
    }

    void Scene::Scale(float scale)
    {
        const mat4 m = glm::scale(mat4(1.0f), vec3(scale));

        for(Object3D &o : mObjects3D) {
            o.SetTransform(o.GetTransform() * m);
        }
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

    AABB Scene::GetShadowCastersAABB() const
    {
        AABB ret;
        for(const auto &obj : mObjects3D) {
            if(obj.mShadowCaster) {
                ret |= obj.GetAABB();
            }
        }
            
        return ret;
    }

    vector<const Object3D*> Scene::GetShadowCasters(const Light *light) const
    {
        std::vector<const engine::Object3D*> ret;
        ret.reserve(mObjects3D.size());

        for(const Object3D &obj : mObjects3D) {
            if(obj.mVisible && obj.mShadowCaster && light->Affects(&obj)) {
                ret.push_back(&obj);
            }
        }

        return ret;
    }
}
