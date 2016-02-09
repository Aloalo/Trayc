/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Scene.h>

namespace engine
{
    Scene::Scene(void)
    {
    }

    AABB Scene::GetAABB() const
    {
        AABB ret;
        for(const auto &obj : mObjects3D)
            ret |= obj.GetAABB();
        return ret;
    }
}
