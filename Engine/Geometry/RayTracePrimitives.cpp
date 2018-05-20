#include <Engine/Geometry/RayTracePrimitives.h>

using namespace glm;

namespace engine
{
    static inline bool IntersectSphere(const Ray &ray, const vec4 &positionRadius, float &minLambda)
    {
        const vec3 L = vec3(positionRadius) - ray.mOrigin;
        const float t = dot(L, ray.mDirection);

        if (t < 0.0) {
            return false;
        }

        const float d2 = dot(L, L) - t * t;

        const float radius2 = positionRadius.w * positionRadius.w;
        if (d2 > radius2) {
            return false;
        }

        const float lambda = t - sqrt(radius2 - d2);
        if (lambda > minLambda) {
            return false;
        }

        minLambda = lambda;
        return true;
    }

    bool RTSphere::Intersect(const Ray &ray, float &minLambda) const
    {
        return IntersectSphere(ray, positionRadius, minLambda);
    }

    bool RTLight::Intersect(const Ray &ray, float &minLambda) const
    {
        return IntersectSphere(ray, positionRadius, minLambda);
    }

    bool RTRectangle::Intersect(const Ray &ray, float &minLambda) const
    {
        const float lambda = (offset - ray.mOrigin[normal]) / ray.mDirection[normal];

        if (lambda < 0.0 || lambda > minLambda) {
            return false;
        }

        const vec3 hitPoint = ray.mOrigin + lambda * ray.mDirection;
        // The values for the X-Z plane should be swizzled for Rectangle.rect.xy, Rectangle.rect.zw
        const vec2 planePoint = vec2(hitPoint[(normal + 1) % 3], hitPoint[(normal + 2) % 3]);

        if (any(lessThan(planePoint, vec2(rect))) || any(greaterThan(planePoint, vec2(rect.z, rect.w)))) {
            return false;
        }

        minLambda = lambda;
        return true;
    }

    bool RTBox::Intersect(const Ray &ray, float &minLambda) const
    {
        const vec3 tmin = (minv - ray.mOrigin) / ray.mDirection;
        const vec3 tmax = (maxv - ray.mOrigin) / ray.mDirection;

        const vec3 near = min(tmin, tmax);
        const vec3 far = max(tmin, tmax);
        const float lmax = min(min(far.x, far.y), far.z);
        float lmin = max(max(near.x, near.y), near.z);

        lmin = mix(lmax, lmin, static_cast<float>(lmin > 0.0f));

        if (lmax < 0.0 || lmax < lmin || lmin > minLambda) {
            return false;
        }

        minLambda = lmin;
        return true;
    }
}
