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
        const vec3 v1 = vec3(p2) - vec3(p1);
        const vec3 v2 = vec3(p3) - vec3(p2);
        const vec3 normal = normalize(cross(v1, v2));

        const float lambda = (dot(normal, vec3(p1)) - dot(ray.mOrigin, normal)) / dot(ray.mDirection, normal);

        if (lambda < 0.0 || lambda > minLambda) {
            return false;
        }

        const vec3 hitPoint = ray.mOrigin + lambda * ray.mDirection;
        const vec3 v4 = hitPoint - vec3(p1);
        const vec3 v5 = hitPoint - vec3(p3);
        // p4 = p1 + p3 - p2
        const vec3 v6 = hitPoint - vec3(p1) - vec3(p3) + vec3(p2);

        if (dot(v1, v4) < 0.0 || dot(v1, v5) > 0.0 || dot(v2, v4) < 0.0 || dot(v2, v6) > 0.0) {
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
