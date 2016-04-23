
#include <Engine/Geometry/Plane.h>

using namespace glm;

namespace engine
{
    Plane::Plane(const vec3 &v1, const vec3 &v2, const vec3 &v3)
    {
        Set(v1, v2, v3);
    }

    Plane::Plane(void)
    {
        Set(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f));
    }


    void Plane::Set(const vec3 &v1, const vec3 &v2, const vec3 &v3)
    {
        const vec3 aux1 = v1 - v2;
        const vec3 aux2 = v3 - v2;

        mNormal = normalize(cross(aux2, aux1));
        mPoint = v2;
        mD = -dot(mNormal, mPoint);
    }

    void Plane::Set(const vec3 &normal, const vec3 &point)
    {
        mNormal = normalize(normal);
        mPoint = point;
        mD = -dot(mNormal, mPoint);
    }

    void Plane::Set(float a, float b, float c, float d) {

        // Set the normal vector
        mNormal = vec3(a, b, c);
        // Compute the lenght of the vector
        const float invLength = 1.0f / length(mNormal);
        // Normalize the vector
        mNormal *= invLength;
        // Divide d by the length as well
        mD = d * invLength;
    }

    float Plane::Distance(const vec3 &p) const
    {
        return mD + dot(mNormal, p);
    }
}
