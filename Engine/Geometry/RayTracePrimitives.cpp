#include <Engine/Geometry/RayTracePrimitives.h>

namespace engine
{
    bool RTSphere::Intersect(const Ray &ray, float &lambda) const
    {
        return false;
    }

    bool RTLight::Intersect(const Ray &ray, float &lambda) const
    {
        return false;
    }

    bool RTRectangle::Intersect(const Ray &ray, float &lambda) const
    {
        return false;
    }

    bool RTBox::Intersect(const Ray &ray, float &lambda) const
    {
        return false;
    }
}
