#ifndef EN_FRUSTUM_H
#define EN_FRUSTUM_H

#include <Engine/Geometry/Plane.h>
#include <Engine/Geometry/AABB.h>
#include <Engine/Geometry/Sphere.h>

namespace engine
{
    class Frustum
    {
    public:
        // Sets perspective frustum, fov is in degrees
        Frustum(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u, float fov, float ar, float near, float far);

        // Sets perspective frustum, fov is in degrees
        void Set(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u, float fov, float ar, float near, float far);
        bool Intersect(const glm::vec3 &p) const;
        bool Intersect(const Sphere &s) const;
        bool Intersect(const AABB &b) const;
    private:
        enum
        {
            TOP = 0,
            BOTTOM,
            LEFT,
            RIGHT,
            NEARP,
            FARP
        };

        Plane mPlanes[6];
    };
}


#endif