
#ifndef EN_SPHERE_H
#define EN_SPHERE_H

#include <glm/glm.hpp>

namespace engine
{
    struct Sphere
    {
        // center = O, radius = 1
        Sphere(void);
        Sphere(const glm::vec3 center, float radius);

        glm::vec3 mCenter;
        float mRadius;
    };
}

#endif
