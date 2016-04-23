
#include <Engine/Geometry/Sphere.h>

using namespace glm;

namespace engine
{
    Sphere::Sphere(void)
        : mCenter(0.0f), mRadius(1.0f)
    {
    }

    Sphere::Sphere(const vec3 center, float radius)
        : mCenter(center), mRadius(radius)
    {
    }
}

